#include "pch.h"
#include "UploadBuffer.h"
#include "Util.h"
#include "GlobalFunctions.h"

#include "../../FBCommon/Utility.h"
using namespace fb;

bool UploadBuffer::Initialize(UINT elementSize, UINT align, UINT count)
{
	ElementSizeBeforeAligned = elementSize;
	if (align != 1 && align != 0) {
		elementSize = fb::CalcAligned(elementSize, align);
	}
	ElementSize = elementSize;
	Count = count;
	auto device = GetDevice();
	try {
		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ElementSize * Count),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&Resource)));

		ThrowIfFailed(Resource->Map(0, nullptr, reinterpret_cast<void**>(&MappedData)));
	}
	catch (const DxException& ex) {
		ex.PrintErrorMessage();
		DebugBreak();
		return false;
	}
	return true;
}

UploadBuffer::~UploadBuffer()
{
	if (Resource != nullptr)
		Resource->Unmap(0, nullptr);

	MappedData = nullptr;
}

void UploadBuffer::CopyData(UINT elementIndex, void* elementData)
{
	memcpy(&MappedData[elementIndex * ElementSize], elementData, ElementSizeBeforeAligned);
}

//void UploadBuffer::CreateCBV(UINT elementIndex, EDescriptorHeapType heapType, UINT heapIndex)
//{
//	assert(heapType == EDescriptorHeapType::Default);
//	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = Resource->GetGPUVirtualAddress();
//	cbAddress += (UINT64)elementIndex * (UINT64)ElementSize;
//	
//	auto descriptorHeap = gRendererD3D12->GetDefaultDescriptorHeap();
//	auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeap->GetCPUDescriptorHandleForHeapStart());
//	handle.Offset(heapIndex, gRendererD3D12->GetCbvSrvUavDescriptorSize());
//	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
//	cbvDesc.BufferLocation = cbAddress;
//	cbvDesc.SizeInBytes = ElementSize;
//	gRendererD3D12->GetDevice()->CreateConstantBufferView(&cbvDesc, handle);
//}