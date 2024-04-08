#pragma once

#include "render/frame_graph.h"
#include "render/resource/frame_graph_resource.h"

namespace orion
{
	/**
	 * @brief
	 */
	struct frame_graph_builder_t
	{
		private:
			frame_graph_t _graph;

		public:

			struct no_data_t {};
			template <typename Data = no_data_t, typename Setup, typename Execute> 
			const Data& add_callback(Setup&& setup, Execute&& execute);

			/**
			 * @brief Declares creation of a resource.
			 *
			 * @tparam T Type of the resource, ex. frame_graph_texture_t.
			 *
			 * @param desc descriptor of a resource.
			 * @return @TODO
			 */
			template <typename T>
			frame_graph_resource_id create(const typename T::desc_t& desc);

			/**
			 * @brief
			 * 
			 * @param
			 * @param
			 * @return @TODO
			 */
			[[nodiscard]] frame_graph_resource_id read(frame_graph_resource_id input);

			/**
			 * @brief
			 * 
			 * @param
			 * @param
			 * @return @TODO
			 */
			[[nodiscard]] frame_graph_resource_id write(frame_graph_resource_id input);

			/**
			 * @brief 
			 *
			 * @return
			 */
			frame_graph_t build() const;
	};
}
