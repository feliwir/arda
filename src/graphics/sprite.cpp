#include "sprite.hpp"
#include "renderer.hpp"

arda::Sprite::Sprite(IRenderer& renderer) : Sprite(renderer,nullptr, 
	{ {-1.0,-1.0 },{ 1.0,-1.0 },{ -1.0, 1.0 },{ 1.0,1.0 } },	//positions
	{ { 0.0, 0.0 },{ 1.0, 0.0 },{  0.0, 1.0 },{ 1.0,1.0 } })	//coords
{
}

arda::Sprite::Sprite(IRenderer& renderer,std::shared_ptr<ITexture> tex, std::shared_ptr<ITexture> mask) : 
	Sprite(renderer, tex,
	{ { -1.0,-1.0 },{ 1.0,-1.0 },{ -1.0, 1.0 },{ 1.0,1.0 } },	//positions
	{ {  0.0, 0.0 },{ 1.0, 0.0 },{  0.0, 1.0 },{ 1.0,1.0 } },	//coords
	mask)
{
}

arda::Sprite::Sprite(IRenderer & renderer, std::shared_ptr<ITexture> tex, std::vector<glm::vec2> positions, 
	std::shared_ptr<ITexture> mask) : Sprite(renderer, tex,
	positions,												//positions
	{ { 0.0, 0.0 },{ 1.0, 0.0 },{ 0.0, 1.0 },{ 1.0,1.0 } },	//coords
mask)
{
}

arda::Sprite::Sprite(IRenderer & renderer, std::shared_ptr<ITexture> tex, std::vector<glm::vec2> positions, 
	std::vector<glm::vec2> coords, std::shared_ptr<ITexture> mask) : m_texture(tex), m_mask(mask), m_opacity(1.0)
{
	m_vertices = renderer.CreateBuffer(IBuffer::VERTEX_BUFFER,
		IBuffer::STATIC);

	std::vector<Vertex> vertices(4);
	int i = 0;
	for (auto& vertex : vertices)
	{
		vertex.Position = positions[i];
		vertex.Coords = coords[i];
		++i;
	}

	m_vertices->Upload(vertices);

	m_indices = renderer.CreateBuffer(IBuffer::INDEX_BUFFER,
		IBuffer::STATIC);

	std::vector<uint16_t> indices = { 0,1,2,3,1,2 };
	m_indices->Upload(indices);

	m_layout = renderer.CreateLayout();
	m_layout->Bind();
	m_layout->Enable(0);
	m_layout->VertexPointer(0, 2, Layout::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	m_layout->Enable(1);
	m_layout->VertexPointer(1, 2, Layout::FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Coords));
}

arda::Sprite::~Sprite()
{
}

void arda::Sprite::Render(IRenderer& renderer)
{
	m_layout->Bind();
	m_texture->Bind();
	renderer.GetActiveShader()->SetFloatProperty("opacity", m_opacity);

	renderer.Draw(m_vertices, m_indices);
}
