#ifndef __MD5_LOADER_HPP_INCLUDED__
#define __MD5_LOADER_HPP_INCLUDED__

namespace indigo
{
class md5_loader : public resource_loader<mesh>
{
public:
	bool can_load(std::string const& extension) const override;
	std::shared_ptr<mesh> load(std::istream& stream) override;
};
}

#endif
