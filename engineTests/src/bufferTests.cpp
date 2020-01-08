#include "bufferTests.h"

TEST(VertexBuffer, Stride)
{
	Engine::BufferLayout layout;

	//layout.addElement(type1);
	//layout.addElement(type2);
	//layout.addElement(type3);

	unsigned int stride = 0;
	stride += Engine::ShaderDataTypeSize(type1);
	stride += Engine::ShaderDataTypeSize(type2);
	stride += Engine::ShaderDataTypeSize(type3);

	EXPECT_EQ(stride, layout.getStride());
}

TEST(VertexBuffer, Offsets)
{

}

TEST(VertexBuffer, Sizes)
{

}

TEST(VertexBuffer, Layout)
{

}