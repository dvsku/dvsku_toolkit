#pragma once

#ifndef DVSKU_TOOLKIT_TAB_DECRYPT_H
#define DVSKU_TOOLKIT_TAB_DECRYPT_H

#include "tab_base.h"

#include "lib/libevp/libevp.h"

#include "helpers/color.h"
#include "helpers/file_dialog.h"

namespace dvsku::toolkit::views {
	class tab_decrypt : public tab_base {
		protected:

		public:
			tab_decrypt() : tab_base() {}

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
				if (Button("Select##DecryptInput", ImVec2(125, 21))) {
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
				if (Button("Select##DecryptOutput", ImVec2(125, 21))) {
					file_dialog::select_folder(m_output);
				}

				offset_draw(20, 15);
				Text("Decryption key");

				offset_draw(20, 20);
				SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
				PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 4));
				InputText(" ", &m_key, ImGuiInputTextFlags_CharsNoBlank);
				PopStyleVar();

				offset_draw(20, 15);
				SetNextItemWidth(MAIN_WINDOW_WIDTH - 65);
				ProgressBar(0, ImVec2(0.0f, 0.0f));

				offset_draw(MAIN_WINDOW_WIDTH / 2 - 125 / 2 - 18, 15);
				
				if (!(*disabled)) {
					if (Button("Decrypt", ImVec2(125, 21))) {
						m_cancel = false;
						dvsku::crypt::libdvsku_crypt crypt(m_key.c_str());

						crypt.decrypt_and_decompress_folder_async(m_input, m_output, &m_cancel,
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
					if (Button("Cancel##Decrypt", ImVec2(125, 21)))
						m_cancel = true;
				}
				
				PopStyleColor();
				PopStyleVar();
			}
	};
}

#endif