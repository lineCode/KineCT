#include "KinectCT.h"


// kinect ct
namespace KineCT {
    // type
    const AMOVIESETUP_MEDIATYPE AMSMediaTypesKCam = {
        &MEDIATYPE_Video,
        &MEDIASUBTYPE_NULL
    };
    // pin
    const AMOVIESETUP_PIN PinsKCam[] = {
        {
            L"Image Output",        // Pin string name
            FALSE,                  // Is it rendered
            TRUE,                   // Is it an output
            FALSE,                  // Can we have none
            FALSE,                  // Can we have many
            &CLSID_NULL,            // Connects to filter
            NULL,                   // Connects to pin
            1,                      // Number of types
            &AMSMediaTypesKCam      // Pin Media types
        } 
    };
    // filter
    const AMOVIESETUP_FILTER AMSFilterKCam =  {
        &CLSID_KVC,                     // Filter CLSID
        L"Kinect Camera Transformer",   // String name
        MERIT_DO_NOT_USE,               // Filter merit
        1,                              // Number pins
        PinsKCam                        // Pin details
    };
    // 注册Filters
    auto RegisterFilters(BOOL bRegister) noexcept { 
        WCHAR achFileName[MAX_PATH];
        ASSERT(g_hInst != 0);
        // 检查路劲
        if (!::GetModuleFileNameW(g_hInst, achFileName, lengthof(achFileName))) {
            return AmHresultFromWin32(::GetLastError());
        }
        // 初始化COM
        auto hr = ::CoInitialize(0);
        if (SUCCEEDED(hr)) {
            IFilterMapper2* mapper = nullptr;
            // Filter Mapper
            hr = ::CoCreateInstance(
                CLSID_FilterMapper2, 
                nullptr, 
                CLSCTX_INPROC_SERVER, 
                IID_IFilterMapper2, 
                reinterpret_cast<void**>(&mapper)
                );
            // 设置
            assert(SUCCEEDED(hr));
            if (SUCCEEDED(hr)) {
                ::AMovieSetupRegisterFilter2(&KineCT::AMSFilterKCam, mapper, bRegister);
            }
            KineCT::SafeRelease(mapper);
        }
        return hr;
    }
}

// 注册服务
STDAPI DllRegisterServer() {
    return KineCT::RegisterFilters(TRUE);
}

// 注销服务
STDAPI DllUnregisterServer() {
    return KineCT::RegisterFilters(FALSE);
}

// base classes 里面的入口函数
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

// Dll 主函数
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  dwReason, LPVOID lpReserved) {

    return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}

#ifdef _DEBUG
#pragma comment(lib, "../Debug/BaseClasses")
#else
#pragma comment(lib, "../Release/BaseClasses")
#endif

