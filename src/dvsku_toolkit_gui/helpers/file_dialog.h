#pragma once

#ifndef DVSKU_TOOLKIT_HELPERS_FILESYSTEM_H
#define DVSKU_TOOLKIT_HELPERS_FILESYSTEM_H

#define STRICT_TYPED_ITEMIDS
#include <shlobj.h>
#include <objbase.h>
#include <shobjidl.h>
#include <shlwapi.h>
#include <knownfolders.h>
#include <propvarutil.h>
#include <propkey.h>
#include <propidl.h>
#include <strsafe.h>
#include <shtypes.h>
#include <new>

#pragma comment(lib,"comctl32.lib") 
#pragma comment(lib,"propsys.lib")
#pragma comment(lib,"shlwapi.lib") 

namespace dvsku::toolkit {
    class file_dialog {
        private:
            static constexpr COMDLG_FILTERSPEC c_rgTypes[] = {
                {L"EVP File (*.evp)",       L"*.evp"},
                {L"All Files (*.*)",        L"*.*"}
            };

            class event_handler : public IFileDialogEvents, public IFileDialogControlEvents {
                public:
                    static HRESULT CreateInstance(REFIID riid, void** ppv) {
                        *ppv = NULL;
                        event_handler* pDialogEventHandler = new (std::nothrow) event_handler();
                        HRESULT hr = pDialogEventHandler ? S_OK : E_OUTOFMEMORY;
                        if (SUCCEEDED(hr))
                        {
                            hr = pDialogEventHandler->QueryInterface(riid, ppv);
                            pDialogEventHandler->Release();
                        }
                        return hr;
                    }

                    IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv) {
                        static const QITAB qit[] = {
                            QITABENT(event_handler, IFileDialogEvents),
                            QITABENT(event_handler, IFileDialogControlEvents),
                            { 0 },
                            #pragma warning(suppress:4838)
                        };
                        return QISearch(this, qit, riid, ppv);
                    }

                    IFACEMETHODIMP_(ULONG) AddRef() {
                        return InterlockedIncrement(&_cRef);
                    }

                    IFACEMETHODIMP_(ULONG) Release() {
                        long cRef = InterlockedDecrement(&_cRef);
                        if (!cRef)
                            delete this;
                        return cRef;
                    }

                    IFACEMETHODIMP OnFileOk(IFileDialog*) { return S_OK; };
                    IFACEMETHODIMP OnFolderChange(IFileDialog*) { return S_OK; };
                    IFACEMETHODIMP OnFolderChanging(IFileDialog*, IShellItem*) { return S_OK; };
                    IFACEMETHODIMP OnHelp(IFileDialog*) { return S_OK; };
                    IFACEMETHODIMP OnSelectionChange(IFileDialog*) { return S_OK; };
                    IFACEMETHODIMP OnShareViolation(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*) { return S_OK; };
                    IFACEMETHODIMP OnOverwrite(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*) { return S_OK; };

                    IFACEMETHODIMP OnTypeChange(IFileDialog* pfd) {
                        IFileSaveDialog* pfsd;
                        HRESULT hr = pfd->QueryInterface(&pfsd);
                        return hr;
                    }

                    IFACEMETHODIMP OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem) {
                        IFileDialog* pfd = NULL;
                        HRESULT hr = pfdc->QueryInterface(&pfd);
                        if (SUCCEEDED(hr)) {
                            pfd->Release();
                        }
                        return hr;
                    }

                    IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize*, DWORD) { return S_OK; };
                    IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize*, DWORD, BOOL) { return S_OK; };
                    IFACEMETHODIMP OnControlActivating(IFileDialogCustomize*, DWORD) { return S_OK; };

                    event_handler() : _cRef(1) {};
            
                private:
                    ~event_handler() {};
                    long _cRef;
            };

        public:
            static void open_file(char* selected_path) {
                IFileDialog* pfd = NULL;
                HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

                if (SUCCEEDED(hr)) {
                    IFileDialogEvents* pfde = NULL;
                    hr = event_handler::CreateInstance(IID_PPV_ARGS(&pfde));

                    if (SUCCEEDED(hr)) {
                        DWORD dwCookie;
                        pfd->Advise(pfde, &dwCookie);

                        DWORD dwFlags;
                        pfd->GetOptions(&dwFlags);
                        pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST);
                        pfd->SetFileTypes(ARRAYSIZE(c_rgTypes), c_rgTypes);
                        pfd->SetFileTypeIndex(1);
                        pfd->SetDefaultExtension(L"evp");

                        hr = pfd->Show(NULL);
                        if (SUCCEEDED(hr)) {
                            IShellItem* psiResult;
                            hr = pfd->GetResult(&psiResult);

                            if (SUCCEEDED(hr)) {
                                LPWSTR pszFilePath = NULL;
                                hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                                if (SUCCEEDED(hr)) {
                                    wcstombs(selected_path, pszFilePath, FILE_PATH_MAX);
                                }
                                psiResult->Release();
                            }
                        }
                        pfd->Unadvise(dwCookie);
                    }
                    pfde->Release();
                }
                pfd->Release();
            }

            static void save_file(char* selected_path) {
                IFileDialog* pfd = NULL;
                HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

                if (SUCCEEDED(hr)) {
                    IFileDialogEvents* pfde = NULL;
                    hr = event_handler::CreateInstance(IID_PPV_ARGS(&pfde));

                    if (SUCCEEDED(hr)) {
                        DWORD dwCookie;
                        pfd->Advise(pfde, &dwCookie);

                        DWORD dwFlags;
                        pfd->GetOptions(&dwFlags);
                        pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST);
                        pfd->SetFileTypes(ARRAYSIZE(c_rgTypes), c_rgTypes);
                        pfd->SetFileTypeIndex(1);
                        pfd->SetDefaultExtension(L"evp");
                        
                        pfd->Show(NULL);
                        if (SUCCEEDED(hr)) {
                            IShellItem* psiResult;
                            hr = pfd->GetResult(&psiResult);

                            if (SUCCEEDED(hr)) {
                                LPWSTR pszFilePath = NULL;
                                hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                                if (SUCCEEDED(hr)) {
                                    wcstombs(selected_path, pszFilePath, FILE_PATH_MAX);
                                }
                                psiResult->Release();
                            }
                        }
                        pfd->Unadvise(dwCookie);
                    }
                    pfde->Release();
                }
                pfd->Release();
            }

            static void select_folder(char* selected_path) {
                IFileDialog* pfd = NULL;
                HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

                if (SUCCEEDED(hr)) {
                    IFileDialogEvents* pfde = NULL;
                    hr = event_handler::CreateInstance(IID_PPV_ARGS(&pfde));

                    if (SUCCEEDED(hr)) {
                        DWORD dwCookie;
                        pfd->Advise(pfde, &dwCookie);

                        DWORD dwFlags;
                        pfd->GetOptions(&dwFlags);
                        pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM | FOS_PICKFOLDERS | FOS_PATHMUSTEXIST);
                        pfd->SetFileTypes(ARRAYSIZE(c_rgTypes), c_rgTypes);
                        pfd->SetFileTypeIndex(1);
                        pfd->SetDefaultExtension(L"evp");
                        
                        hr = pfd->Show(NULL);
                        if (SUCCEEDED(hr)) {
                            IShellItem* psiResult;
                            hr = pfd->GetResult(&psiResult);

                            if (SUCCEEDED(hr)) {
                                LPWSTR pszFilePath = NULL;
                                hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                                if (SUCCEEDED(hr)) {
                                    wcstombs(selected_path, pszFilePath, FILE_PATH_MAX);
                                }
                                psiResult->Release();
                            }
                        }
                        pfd->Unadvise(dwCookie);
                    }
                    pfde->Release();
                }
                pfd->Release();
            }

    };
}

#endif