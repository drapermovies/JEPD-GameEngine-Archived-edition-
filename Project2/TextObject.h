#pragma once
#include "FontClass.h"
#include "BufferTypes.h"
#include "ShaderManager.h"

class TextObject
{
public:
	TextObject() = default;
	TextObject(const TextObject&) = default;
	~TextObject() = default;

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, int, FontClass*,
					char*, int, int, float, float, float);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, ShaderManager*,
				DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX,
			    ID3D11ShaderResourceView*);
private:
	struct SentenceType
	{
		ID3D11Buffer* vertex_buffer;
		ID3D11Buffer* index_buffer;
		int vertex_count, index_count, max_length;
		float red, green, blue;
	};

	bool InitializeSentence(ID3D11Device*, ID3D11DeviceContext*, FontClass*, char*, int, int, 
						    float, float, float);
	bool UpdateSentence(ID3D11DeviceContext*, FontClass*, char*, int, int, float, float, float);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, ShaderManager*, DirectX::XMMATRIX, DirectX::XMMATRIX,
					    DirectX::XMMATRIX, ID3D11ShaderResourceView*);

	int m_ScreenHeight = -1, m_ScreenWidth = -1;
	int m_MaxLength = -1;

	SentenceType* sentence = nullptr;
};