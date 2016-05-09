//
// Created by Joseph Birkner on 5/7/16.
//

#include "scene.hpp"
#include "group_entity.hpp"
#include "basic_light.hpp"

using namespace indigo;

struct scene::impl {
    impl(glm::vec3 const&) {}
    group_entity root_entity;
    global_light sun;
};

scene::scene(glm::vec3 const& size) : impl_(new impl(size)) {

}

scene::~scene() {

}

scene& scene::add(entity_shared_ptr ent) {
    impl_->root_entity.add(ent);
    return *this;
}

scene& scene::remove(entity_shared_ptr const& ent) {
    return *this;
}

entity& scene::root() {
    return impl_->root_entity;
}

global_light const &scene::sun() {
    return impl_->sun;
}

void scene::sun(global_light const &s) {
    impl_->sun = s;
}





