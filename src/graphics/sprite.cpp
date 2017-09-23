#include "sprite.hpp"
#include "renderer.hpp"

arda::Sprite::Sprite(IRenderer& renderer)
{
	m_positions = renderer.CreateBuffer(Buffer::VERTEX_BUFFER,
										Buffer::STATIC);

	std::vector<Vertex> positions = { { {-1.0,-1.0} },{ { 1.0,-1.0 } }
									 ,{ {-1.0, 1.0} },{ { 1.0, 1.0 } } };

	m_positions->Upload(positions);

	m_indices = renderer.CreateBuffer(Buffer::INDEX_BUFFER,
									  Buffer::STATIC);

	std::vector<uint16_t> indices = { 0,1,2,3,1,2 };
	m_indices->Upload(indices);

	m_layout = renderer.CreateLayout();
	m_layout->Bind();
	m_layout->Enable(0);
	m_layout->VertexPointer(0, 2, Layout::FLOAT, 2 * sizeof(float), (void*)offsetof(Vertex, Position));
}

arda::Sprite::Sprite(IRenderer& renderer,std::shared_ptr<ITexture> tex) :
	Sprite(renderer)
{
	m_texture = tex;
}

arda::Sprite::~Sprite()
{
}

void arda::Sprite::Render(IRenderer& renderer)
{
	m_layout->Bind();
	m_texture->Bind();
	renderer.Draw(m_positions, m_indices);
}
