#pragma once
#include "App.h"
#include <memory>  // for std::unique_ptr

int main(int argc, char** argv) {
    // 使用智能指针管理 App 对象，自动处理内存释放
    std::unique_ptr<Opengl::App> app = std::make_unique<Opengl::App>();

    // 运行应用程序
    app->Run();

    // 当 main 函数结束时，智能指针会自动销毁对象，无需显式 delete
    return 0;
}