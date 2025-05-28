#pragma once
#include "pch.h"
#include "application.h"

extern fuse::app_config fuse::create_application();  

int main(int argc, char** argv) {   
#ifdef _DEBUG
MessageBoxA(NULL, "AllocConsole failed!", "Error", MB_OK);
    #ifdef _WIN32
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        SetConsoleOutputCP(CP_UTF8);
    #else
        // 非Windows平台，直接输出到终端
        // 可选：输出提示
        printf("[DEBUG] 控制台已启用\n");
    #endif
#endif

  auto config = fuse::create_application();
  fuse::run_application(config);

#ifdef _DEBUG
    #ifdef _WIN32
        system("pause");
    #else
        printf("按回车键退出...\n");
        fflush(stdout);
        getchar();
    #endif
#endif

  return 0;
}