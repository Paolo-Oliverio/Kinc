#include "pch.h"

#include "Direct3D11.h"

#include <kinc/graphics4/vertexbuffer.h>

#include <Kore/SystemMicrosoft.h>

void kinc_g4_vertex_buffer_init(kinc_g4_vertex_buffer_t *buffer, int count, kinc_g4_vertex_structure_t *structure, kinc_g4_usage_t usage,
                                int instance_data_step_rate) {
	buffer->impl.lockStart = 0;
	buffer->impl.count = count;
	buffer->impl.stride = 0;
	for (int i = 0; i < structure->size; ++i) {
		switch (structure->elements[i].data) {
		case KINC_G4_VERTEX_DATA_FLOAT1:
			buffer->impl.stride += 1 * 4;
			break;
		case KINC_G4_VERTEX_DATA_FLOAT2:
			buffer->impl.stride += 2 * 4;
			break;
		case KINC_G4_VERTEX_DATA_FLOAT3:
			buffer->impl.stride += 3 * 4;
			break;
		case KINC_G4_VERTEX_DATA_FLOAT4:
			buffer->impl.stride += 4 * 4;
			break;
		case KINC_G4_VERTEX_DATA_COLOR:
			buffer->impl.stride += 1 * 4;
			break;
		case KINC_G4_VERTEX_DATA_FLOAT4X4:
			buffer->impl.stride += 4 * 4 * 4;
			break;
		case KINC_G4_VERTEX_DATA_SHORT2_NORM:
			buffer->impl.stride += 2 * 2;
			break;
		case KINC_G4_VERTEX_DATA_SHORT4_NORM:
			buffer->impl.stride += 4 * 2;
			break;
		}
	}

	if (buffer->impl.usage == KINC_G4_USAGE_READABLE) {
		buffer->impl.vertices = (float*)malloc(buffer->impl.stride * count);
	} else {
		buffer->impl.vertices = nullptr;
	}

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.CPUAccessFlags = 0;

	buffer->impl.usage = usage;
	switch (usage) {
	case KINC_G4_USAGE_STATIC:
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		break;
	case KINC_G4_USAGE_DYNAMIC:
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case KINC_G4_USAGE_READABLE:
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		break;
	}

	bufferDesc.ByteWidth = buffer->impl.stride * count;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	kinc_microsoft_affirm(device->CreateBuffer(&bufferDesc, nullptr, &buffer->impl.vb));
}

void kinc_g4_vertex_buffer_destroy(kinc_g4_vertex_buffer_t *buffer) {
	buffer->impl.vb->Release();
	if (buffer->impl.usage == KINC_G4_USAGE_READABLE) {
		free(buffer->impl.vertices);
	}
}

float *kinc_g4_vertex_buffer_lock_all(kinc_g4_vertex_buffer_t *buffer) {
	return kinc_g4_vertex_buffer_lock(buffer, 0, buffer->impl.count);
}

float *kinc_g4_vertex_buffer_lock(kinc_g4_vertex_buffer_t *buffer, int start, int count) {
	switch (buffer->impl.usage) { 
		case KINC_G4_USAGE_STATIC : 
			buffer->impl.lockStart = start;
		    buffer->impl.lockCount = count;
		    D3D11_MAPPED_SUBRESOURCE mappedResource;
		    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		    context->Map(buffer->impl.vb, 0, D3D11_MAP_WRITE, 0, &mappedResource);
		    buffer->impl.vertices = (float *)(mappedResource.pData);
			break;
	    case KINC_G4_USAGE_DYNAMIC :
			if (buffer->impl.lockStart == 0) {
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
				context->Map(buffer->impl.vb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				//lockstart at 1 equals initialized.
				buffer->impl.lockStart = 1;
				buffer->impl.vertices = (float *)(mappedResource.pData);
			}
		    break;
	    case KINC_G4_USAGE_READABLE :
		    buffer->impl.lockStart = start;
		    buffer->impl.lockCount = count;
		    break;
	}
	return &buffer->impl.vertices[start * buffer->impl.stride / 4];
}

void kinc_g4_vertex_buffer_unlock_all(kinc_g4_vertex_buffer_t *buffer) {
	kinc_g4_vertex_buffer_unlock(buffer, buffer->impl.lockCount);
}

void kinc_g4_vertex_buffer_unlock(kinc_g4_vertex_buffer_t *buffer, int count) {
	switch (buffer->impl.usage) {
		case KINC_G4_USAGE_STATIC:
			context->Unmap(buffer->impl.vb, 0);
			buffer->impl.vertices = nullptr;
			break;
		case KINC_G4_USAGE_DYNAMIC:
			context->Unmap(buffer->impl.vb, 0);
			buffer->impl.lockStart = 0;
			buffer->impl.vertices = nullptr;
			break;
		case KINC_G4_USAGE_READABLE:
			context->UpdateSubresource(buffer->impl.vb, 0, nullptr, buffer->impl.vertices, 0, 0);//TODO check commented implementation(untested) as this copyes everything.
			/*D3D11_BOX box{};
		    box.left = buffer->impl.lockStart * buffer->impl.stride;
		    box.right = buffer->impl.lockCount * buffer->impl.stride;
		    box.top = 0;
		    box.bottom = 1;
		    box.front = 0;
		    box.back = 1;
		    context->UpdateSubresource(buffer->impl.vb, 0, &box , &buffer->impl.vertices[buffer->impl.lockStart * buffer->impl.stride / 4], 0, 0);*/
			break;
	}
}

int kinc_internal_g4_vertex_buffer_set(kinc_g4_vertex_buffer_t *buffer, int offset) {
	// UINT stride = myStride;
	// UINT internaloffset = 0;
	// context->IASetVertexBuffers(0, 1, &vb, &stride, &internaloffset);
	return 0;
}

int kinc_g4_vertex_buffer_count(kinc_g4_vertex_buffer_t *buffer) {
	return buffer->impl.count;
}

int kinc_g4_vertex_buffer_stride(kinc_g4_vertex_buffer_t *buffer) {
	return buffer->impl.stride;
}
