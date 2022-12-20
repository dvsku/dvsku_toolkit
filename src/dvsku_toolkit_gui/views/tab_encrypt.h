#pragma once

#ifndef DVSKU_TOOLKIT_TAB_ENCRYPT_H
#define DVSKU_TOOLKIT_TAB_ENCRYPT_H

#include "tab_base.h"

#include "lib/libevp/libevp.h"
#include "lib/libdvsku_crypt/libdvsku_crypt.h"

#include "helpers/color.h"
#include "helpers/file_dialog.h"

namespace dvsku::toolkit::views {
	class tab_encrypt : public tab_base {
		protected:
			bool m_encrypt_to_folder = true;

		public:
			tab_encrypt() : tab_base() {}

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
				if (Button("Select##EncryptInput", ImVec2(125, 21))) {
					file_dialog::select_folder(m_input);
				}

				offset_draw(20, 15);
				Checkbox("Encrypt to folder", &m_encrypt_to_folder);
				if (IsItemHovered())
					SetTooltip("If checked -> encrypts files and saves them to output folder");

				if (m_encrypt_to_folder) {
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
					if (Button("Select##EncryptOutput", ImVec2(125, 21))) {
						file_dialog::select_folder(m_output);
					}
				}

				offset_draw(20, 15);
				Text("Encryption key");

				offset_draw(20, 20);
				SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
				PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
				InputText(" ", &m_key, ImGuiInputTextFlags_CharsNoBlank);
				PopStyleVar();

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
				
				bool cannot_start = m_input.empty() || (m_encrypt_to_folder && m_output.empty()) || m_key.empty();

				if (cannot_start)
					BeginDisabled();

				if (!(*disabled)) {
					if (Button("Encrypt", ImVec2(125, 21))) {
						m_cancel = false;
						dvsku::crypt::libdvsku_crypt crypt(m_key.c_str());
						
						crypt.encrypt_folder_async(m_input, m_output, FILE_FILTER_NONE, &m_cancel,
							[this, disabled]() {
								*disabled = true;
								m_progress = 0.0f;
							},
							[this](float progress) {
								m_progress += progress;
							},
							[this, disabled](crypt_result result) {
								if (result == CRYPT_OK) {
									m_progress = 100.0f;
									m_input = "";
									m_output = "";
									m_key = "";
								}
								else if (result == CRYPT_CANCELLED)
									m_progress = 0.0f;

								*disabled = false;
							},
							[this, disabled](crypt_result result) {
								*disabled = false;
							}
						);
					}
				}
				else {
					if (Button("Cancel##Encrypt", ImVec2(125, 21)))
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