#include "mesh_entity.hpp"
#include "mesh.hpp"

using namespace indigo;

mesh_entity::mesh_entity(const mesh* m)
	: model_(m)
{}

// const mesh* mesh_entity::model() const
// {
// 	return model_;
// }
// 
// void mesh_entity::model(const mesh* m)
// {
// 	model_ = m;
// }

void mesh_entity::render() const
{
	if (model_)
		model_->render();
}
