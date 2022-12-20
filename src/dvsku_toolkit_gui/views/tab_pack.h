#pragma once

#ifndef DVSKU_TOOLKIT_TAB_PACK_H
#define DVSKU_TOOLKIT_TAB_PACK_H

#include "tab_base.h"

#include "lib/libevp/libevp.h"

#include "helpers/color.h"
#include "helpers/file_dialog.h"

namespace dvsku::toolkit::views {
	class tab_pack : public tab_base {
		protected:
			int m_file_filter = FILE_FILTER_NONE;

			bool m_encrypt = false;

		public:
			tab_pack() : tab_base() {}

			void build(bool* disabled) override {
				PushStyleColor(ImGuiCol_FrameBg, ARGB2UINT("#FF383838"));
				PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 4));

				offset_draw(20, 20);
				Text("Input");

				BeginDisabled(true);

				SetNextItemWidth(MAIN_WINDOW_WIDTH - 70 - 130);
				offset_draw(20, 20);
				PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
				InputText("", &m_input, ImGuiInputTextFlags_ReadOnly);
				PopStyleVar();

				EndDisabled();

				SameLine();
				if (Button("Select##PackInput", ImVec2(125, 21))) {
					file_dialog::select_folder(m_input);
				}

				offset_draw(20, 15);
				Text("Output");

				BeginDisabled(true);

				SetNextItemWidth(MAIN_WINDOW_WIDTH - 70 - 130);
				offset_draw(20, 20);
				PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
				InputText("", &m_output, ImGuiInputTextFlags_ReadOnly);
				PopStyleVar();

				EndDisabled();

				SameLine();
				if (Button("Select##PackOutput", ImVec2(125, 21))) {
					file_dialog::save_file(m_output);
				}

				offset_draw(20, 15);
				Checkbox("Encrypt", &m_encrypt);

				if (m_encrypt) {
					offset_draw(20, 15);
					Text("Encryption key");

					offset_draw(20, 20);
					SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
					PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
					InputText(" ", &m_key, ImGuiInputTextFlags_CharsNoBlank);
					PopStyleVar();
				}

				offset_draw(20, 15);
				Text("Pack type");

				offset_draw(20, 20);
				RadioButton("any", &m_file_filter, (int)(FILE_FILTER_NONE));
				if (IsItemHovered())
					SetTooltip("Pack all files from input");
				SameLine();

				RadioButton("client", &m_file_filter, (int)(FILE_FILTER_CLIENT));
				if (IsItemHovered())
					SetTooltip("Pack only client files");
				SameLine();

				RadioButton("server", &m_file_filter, (int)(FILE_FILTER_SERVER));
				if (IsItemHovered())
					SetTooltip("Pack only server files");

				offset_draw(20, 15);
				SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);

				if (m_cancel)
					strcpy(m_progress_text, "Cancelled");
				else
					sprintf(m_progress_text, "%.2f%", m_progress);

				if (*disabled)
					EndDisabled();

				PushStyleColor(ImGuiCol_PlotHistogram, ARGB2UINT("#FF774F2D"));
				ProgressBar(m_progress / 100, ImVec2(0.0f, 0.0f), m_progress_text);
				PopStyleColor();

				offset_draw(MAIN_WINDOW_WIDTH / 2 - 125 / 2 - 18, 15);

				bool cannot_start = m_input.empty() || m_output.empty() || (m_encrypt && m_key.empty());

				if (cannot_start)
					BeginDisabled();

				if (!(*disabled)) {
					if (Button("Pack", ImVec2(125, 21))) {
						m_cancel = false;
						dvsku::evp::pack_async(m_input, m_output, m_encrypt, m_key,
							(dvsku::filesys::utilities::file_filter)m_file_filter, &m_cancel,
							[this, disabled]() {
								*disabled = true;
								m_progress = 0.0f;
							},
							[this](float progress) {
								m_progress += progress;
							},
							[this, disabled](dvsku::evp::evp_status status) {
								if (status == dvsku::evp::evp_status::ok) {
									m_progress = 100.0f;
									m_input = "";
									m_output = "";
									m_key = "";
								}
								else if (status == dvsku::evp::evp_status::cancelled)
									m_progress = 0.0f;

								*disabled = false;
							},
							[this, disabled](dvsku::evp::evp_result result) {
								*disabled = false;
							}
						);
					}
				}
				else {
					if (Button("Cancel##Pack", ImVec2(125, 21)))
						m_cancel = true;
				}

				if (cannot_start)
					EndDisabled();

				if (*disabled)
					BeginDisabled();

				PopStyleColor();
				PopStyleVar();
			}
	};
}

#endif
