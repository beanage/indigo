#include "md5_loader.hpp"
#include "md5_bone.hpp"
#include "md5_mesh.hpp"
#include "md5_model.hpp"
#include "util/switch_t.hpp"
#include "platform/gl.hpp"
#include <fstream>
#include <array>

// md5 format
// ----------
//
// MD5Version 10
// commandline "..."
//
// numJoints n
// numMeshes n
//
// joints {
//     "name" parent ( pos.x pos.y pos.z ) ( orient.x orient.y orient.z )
// }
//
// mesh {
//     shader "material"
//
//     numverts n
//     vert index ( u v ) start_weight weight_count
//     vert ...
//
//     numtris n
//     tri index vert1 vert2 vert3
//     tri ...
//
//     numweights n
//     weight index joint bias ( pos.x pos.y pos.z )
//     weight ...
// }

using namespace indigo;

class md5_loader::scanner
{
public:
    struct token
    {
        token()
            : type(eof), num_val(0.f)
        {
        }

        token(unsigned t)
            : type(t), num_val(0.f)
        {
        }

        enum
        {
            null,
            lparen,
            rparen,
            lbrace,
            rbrace,
            identifier,
            string,
            number,
            eof
        };

        unsigned type;
        std::string str_val;
        float num_val;
    };

    scanner() = delete;
    scanner(std::string const&& source)
        : source_(source), pos_(source_.begin())
    {
    }

    token next()
    {
        skip_whitespace();
        skip_comment();

        if (eof())
            return {token::eof};

        switch (*pos_) {
        case '{':
            ++pos_;
            return {token::lbrace};
        case '}':
            ++pos_;
            return {token::rbrace};
        case '(':
            ++pos_;
            return {token::lparen};
        case ')':
            ++pos_;
            return {token::rparen};

        default:
            if (*pos_ == '"')
                return parse_string();
            else if (isalpha(*pos_))
                return parse_ident();
            else if (isdigit(*pos_) || *pos_ == '-')
                return parse_number();
        }

        return {token::null};
    }

    bool eof() const
    {
        return pos_ == source_.end();
    }

private:
    char get() const
    {
        if (pos_ == source_.cend())
            return '\0';
        return *pos_;
    }

    void skip_whitespace()
    {
        while (!eof() && isspace(*pos_))
            ++pos_;
    }

    void skip_comment()
    {
        if (get() == '/') {
            ++pos_;
            if (get() == '/') {
                while (!eof() && *++pos_ != '\n') {
                }
                if (!eof()) {
                    ++pos_;
                }
                skip_whitespace();
            }
        }
    }

    token parse_string()
    {
        token t;
        t.type = token::string;

        while (!eof() && *++pos_ != '"') {
            if (get() == '\\')
                ++pos_;
            t.str_val.push_back(*pos_);
        }

        if (!eof())
            ++pos_;

        return t;
    }

    token parse_ident()
    {
        token t;
        t.type = token::identifier;

        while (isalnum(get()) || get() == '_')
            t.str_val.push_back(*pos_++);

        return t;
    }

    token parse_number()
    {
        token t;
        t.type = token::number;
        t.num_val = 0.f;

        float neg = 1.f;
        if (get() == '-') {
            neg = -1.f;
            ++pos_;
        }

        unsigned comma = 0;
        while (isdigit(get()) || get() == '.') {
            if (*pos_ == '.' && !comma)
                comma = 1;
            else if (!comma)
                t.num_val = t.num_val * 10.f + (*pos_ - '0');
            else
                t.num_val = t.num_val + (*pos_ - '0') / pow(10, comma++);
            ++pos_;
        }

        t.num_val *= neg;

        return t;
    }

    std::string const source_;
    std::string::const_iterator pos_;
};

namespace {
struct md5_joint
{
    md5_joint()
        : parent_index(-1)
    {
    }

    std::string name;
    int parent_index;
    glm::vec3 position;
    glm::vec3 orientation;
};

struct md5_triangle
{
    std::array<int, 3> verts;
};
}

template <typename T>
static T parse_number(md5_loader::scanner& s)
{
    md5_loader::scanner::token t = s.next();
    if (t.type == md5_loader::scanner::token::number)
        return static_cast<T>(t.num_val);
    throw std::runtime_error("[md5_loader parse_number] expected number!");
}

static std::string parse_string(md5_loader::scanner& s)
{
    md5_loader::scanner::token t = s.next();
    if (t.type == md5_loader::scanner::token::string)
        return t.str_val;
    throw std::runtime_error("[md5_loader parse_t] expected string!");
}

static void parse_version(md5_loader::scanner& s)
{
    if (parse_number<unsigned>(s) != 10)
        throw std::runtime_error("[md5_loader parse_version] unknown md5 file "
                                 "version. Only version 10 is loadable!");
}

static void parse_commandline(md5_loader::scanner& s)
{
    s.next();
}

static glm::vec3 parse_vec3(md5_loader::scanner& s)
{
    md5_loader::scanner::token t = s.next();
    if (t.type == md5_loader::scanner::token::lparen) {
        float f1 = parse_number<float>(s);
        float f2 = parse_number<float>(s);
        float f3 = parse_number<float>(s);

        glm::vec3 vec(f1, f2, f3);

        s.next();

        return vec;
    }

    throw std::runtime_error("[md5_loader parse_vec3] expected '('!");
}

static void parse_joints(md5_loader::scanner& s,
                         std::vector<md5_joint>& joints)
{
    md5_loader::scanner::token t = s.next();
    if (t.type == md5_loader::scanner::token::lbrace) {
        t = s.next();
        while (t.type != md5_loader::scanner::token::rbrace) {
            md5_joint j;
            j.name = t.str_val;
            j.parent_index = parse_number<int>(s);
            j.position = parse_vec3(s);
            j.orientation = parse_vec3(s);

            joints.push_back(std::move(j));

            t = s.next();
        }
    } else {
        throw std::runtime_error("[md5_loader parse_joints] expected '{'!");
    }
}

static void parse_vert(md5_loader::scanner& s,
                       std::vector<md5_mesh::vertex>& vertices)
{
    md5_loader::scanner::token t = s.next();
    if (t.type != md5_loader::scanner::token::number)
        throw std::runtime_error(
            "[md5_loader parse_vertex] vertex index expected!");

    t = s.next();
    if (t.type != md5_loader::scanner::token::lparen)
        throw std::runtime_error("[md5_loader parse_vertex] expected '('!");

    md5_mesh::vertex v;
    float f1 = parse_number<float>(s);
    float f2 = parse_number<float>(s);
    v.uv = glm::vec2(f1, f2);

    t = s.next();
    if (t.type != md5_loader::scanner::token::rparen)
        throw std::runtime_error("[md5_loader parse_vertex] expected ')'!");

    v.start_weight = parse_number<unsigned>(s);
    v.count_weight = parse_number<unsigned>(s);

    vertices.push_back(std::move(v));
}

static void parse_tri(md5_loader::scanner& s, std::vector<md5_triangle>& tris)
{
    md5_loader::scanner::token t = s.next();
    if (t.type != md5_loader::scanner::token::number)
        throw std::runtime_error(
            "[md5_loader parse_vertex] triangle index expected!");

    md5_triangle tri;
    tri.verts[0] = parse_number<int>(s);
    tri.verts[1] = parse_number<int>(s);
    tri.verts[2] = parse_number<int>(s);

    tris.push_back(std::move(tri));
}

static void parse_weight(md5_loader::scanner& s,
                         std::vector<md5_mesh::weight>& weights)
{
    md5_loader::scanner::token t = s.next();
    if (t.type != md5_loader::scanner::token::number)
        throw std::runtime_error(
            "[md5_loader parse_vertex] weight index expected!");

    md5_mesh::weight w;
    w.joint = parse_number<int>(s);
    w.bias = parse_number<float>(s);
    w.position = parse_vec3(s);

    weights.push_back(std::move(w));
}

static glm::quat build_quat(glm::vec3 const& vec)
{
    glm::quat q;
    q.x = vec.x;
    q.y = vec.y;
    q.z = vec.z;

    float t = 1.0f - (q.x * q.x) - (q.y * q.y) - (q.z * q.z);
    q.w = t < 0.f ? 0.f : -sqrt(t);

    return q;
}

std::unique_ptr<md5_mesh> md5_loader::parse_mesh(scanner& s)
{
    std::string material;
    std::vector<md5_mesh::vertex> tmp_vertices;
    std::vector<md5_triangle> tmp_tris;

    std::unique_ptr<md5_mesh> m(new md5_mesh);

    scanner::token t = s.next();
    if (t.type == scanner::token::lbrace) {
        t = s.next();
        while (t.type != scanner::token::rbrace) {
            if (t.type == scanner::token::identifier) {
                switch_t<std::string>(t.str_val)
                    .case_t("shader", [&]() { material = parse_string(s); })
                    .case_t("numverts",
                            [&]() { tmp_vertices.reserve(parse_number<int>(s)); })
                    .case_t("vert", [&]() { parse_vert(s, tmp_vertices); })
                    .case_t("numtris",
                            [&]() { tmp_tris.reserve(parse_number<int>(s)); })
                    .case_t("tri", [&]() { parse_tri(s, tmp_tris); })
                    .case_t("numweights",
                            [&]() { m->weights_.reserve(parse_number<int>(s)); })
                    .case_t("weight", [&]() { parse_weight(s, m->weights_); });
            } else {
                throw std::runtime_error("[md5_loader::load_mesh] invalid token!");
            }

            t = s.next();
        }
    } else {
        throw std::runtime_error("[md5_loader parse_mesh] expected '{'!");
    }

    // fill the mesh
    for (auto const& tri : tmp_tris) {
        m->vertices_.push_back(tmp_vertices[tri.verts[2]]);
        m->vertices_.push_back(tmp_vertices[tri.verts[1]]);
        m->vertices_.push_back(tmp_vertices[tri.verts[0]]);
    }

    return m;
}

bool md5_loader::can_load(std::string const& extension) const
{
    return extension == "md5mesh";
}

std::shared_ptr<model> md5_loader::load(std::string const& filename)
{
    std::ifstream stream(filename);
    md5_model* m(new md5_model);
    std::shared_ptr<model> result(m);

    std::vector<md5_joint> tmp_bones;

    scanner s(std::string(std::istreambuf_iterator<char>(stream), {}));

    scanner::token t = s.next();
    while (t.type != scanner::token::eof) {
        if (t.type == scanner::token::identifier) {
            switch_t<std::string>(t.str_val)
                .case_t("MD5Version", [&]() { parse_version(s); })
                .case_t("commandline", [&]() { parse_commandline(s); })
                .case_t("numJoints",
                        [&]() { tmp_bones.reserve(parse_number<size_t>(s)); })
                .case_t("numMeshes",
                        [&]() { m->meshes_.reserve(parse_number<size_t>(s)); })
                .case_t("joints", [&]() { parse_joints(s, tmp_bones); })
                .case_t("mesh", [&]() { m->meshes_.push_back(parse_mesh(s)); });
        } else {
            throw std::runtime_error("[md5_loader::load] invalid token!");
        }

        t = s.next();
    }

    // Fill the models bones
    for (auto const& tmpb : tmp_bones) {
        std::unique_ptr<md5_bone> b(new md5_bone);
        b->name_ = tmpb.name;
        b->parent_ = nullptr;
        b->position_ = tmpb.position;
        b->rotation_ = build_quat(tmpb.orientation);

        m->bones_.push_back(std::move(b));
    }

    // Fill the bones parent
    for (int i = 0; i < tmp_bones.size(); ++i) {
        if (tmp_bones[i].parent_index >= 0)
            m->bones_[i]->parent_ = m->bones_[tmp_bones[i].parent_index].get();
    }

    return result;
}
