// SPDX-FileCopyrightText: 2002-2025 PCSX2 Dev Team
// SPDX-License-Identifier: GPL-3.0+

#pragma once

#include "common/Pcsx2Defs.h"
#include "common/RedtapeWindows.h"
#include "common/RedtapeWilCom.h"

#include <array>
#include <d3d12.h>
#include <string_view>

class D3D12ShaderCache;

namespace D3D12
{
	class RootSignatureBuilder
	{
	public:
		enum : u32
		{
			MAX_PARAMETERS = 16,
			MAX_DESCRIPTOR_RANGES = 16
		};

		RootSignatureBuilder();

		void Clear();

		wil::com_ptr_nothrow<ID3D12RootSignature> Create(bool clear = true);

		void SetInputAssemblerFlag();

		u32 Add32BitConstants(u32 shader_reg, u32 num_values, D3D12_SHADER_VISIBILITY visibility);
		u32 AddCBVParameter(u32 shader_reg, D3D12_SHADER_VISIBILITY visibility);
		u32 AddSRVParameter(u32 shader_reg, D3D12_SHADER_VISIBILITY visibility);
		u32 AddDescriptorTable(D3D12_DESCRIPTOR_RANGE_TYPE rt, u32 start_shader_reg, u32 num_shader_regs,
			D3D12_SHADER_VISIBILITY visibility);

	private:
		D3D12_ROOT_SIGNATURE_DESC m_desc{};
		std::array<D3D12_ROOT_PARAMETER, MAX_PARAMETERS> m_params{};
		std::array<D3D12_DESCRIPTOR_RANGE, MAX_DESCRIPTOR_RANGES> m_descriptor_ranges{};
		u32 m_num_descriptor_ranges = 0;
	};

	class GraphicsPipelineBuilder
	{
	public:
		enum : u32
		{
			MAX_VERTEX_ATTRIBUTES = 16,
		};

		GraphicsPipelineBuilder();

		~GraphicsPipelineBuilder() = default;

		void Clear();

		wil::com_ptr_nothrow<ID3D12PipelineState> Create(ID3D12Device* device, bool clear = true);
		wil::com_ptr_nothrow<ID3D12PipelineState> Create(
			ID3D12Device* device, D3D12ShaderCache& cache, bool clear = true);

		void SetRootSignature(ID3D12RootSignature* rs);

		void SetVertexShader(const void* data, u32 data_size);
		void SetGeometryShader(const void* data, u32 data_size);
		void SetPixelShader(const void* data, u32 data_size);

		void SetVertexShader(const ID3DBlob* blob);
		void SetGeometryShader(const ID3DBlob* blob);
		void SetPixelShader(const ID3DBlob* blob);

		void AddVertexAttribute(
			const char* semantic_name, u32 semantic_index, DXGI_FORMAT format, u32 buffer, u32 offset);

		void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type);

		void SetRasterizationState(D3D12_FILL_MODE polygon_mode, D3D12_CULL_MODE cull_mode, bool front_face_ccw);

		void SetMultisamples(u32 multisamples);

		void SetNoCullRasterizationState();

		void SetDepthState(bool depth_test, bool depth_write, D3D12_COMPARISON_FUNC compare_op);
		void SetStencilState(bool stencil_test, u8 read_mask, u8 write_mask, const D3D12_DEPTH_STENCILOP_DESC& front,
			const D3D12_DEPTH_STENCILOP_DESC& back);

		void SetNoDepthTestState();
		void SetNoStencilState();

		void SetBlendState(u32 rt, bool blend_enable, D3D12_BLEND src_factor, D3D12_BLEND dst_factor, D3D12_BLEND_OP op,
			D3D12_BLEND alpha_src_factor, D3D12_BLEND alpha_dst_factor, D3D12_BLEND_OP alpha_op,
			u8 write_mask = D3D12_COLOR_WRITE_ENABLE_ALL);
		void SetColorWriteMask(u32 rt, u8 write_mask = D3D12_COLOR_WRITE_ENABLE_ALL);

		void SetNoBlendingState();

		void ClearRenderTargets();

		void SetRenderTarget(u32 rt, DXGI_FORMAT format);

		void ClearDepthStencilFormat();

		void SetDepthStencilFormat(DXGI_FORMAT format);

	private:
		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_desc;
		std::array<D3D12_INPUT_ELEMENT_DESC, MAX_VERTEX_ATTRIBUTES> m_input_elements;
	};

	class ComputePipelineBuilder
	{
	public:
		ComputePipelineBuilder();
		~ComputePipelineBuilder() = default;

		void Clear();

		wil::com_ptr_nothrow<ID3D12PipelineState> Create(ID3D12Device* device, bool clear = true);
		wil::com_ptr_nothrow<ID3D12PipelineState> Create(
			ID3D12Device* device, D3D12ShaderCache& cache, bool clear = true);

		void SetRootSignature(ID3D12RootSignature* rs);

		void SetShader(const void* data, u32 data_size);

	private:
		D3D12_COMPUTE_PIPELINE_STATE_DESC m_desc;
	};

#ifdef PCSX2_DEVBUILD
	void SetObjectName(ID3D12Object* object, std::string_view name);
#else
	static inline void SetObjectName(ID3D12Object* object, std::string_view name) {}
#endif
} // namespace D3D12
