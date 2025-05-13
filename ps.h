#pragma once

#include <Windows.h>  // 需包含 Windows API 头文件（AlphaBlend 所需）
#pragma comment(lib, "gdi32.lib")  // 需链接 gdi32.lib 库（AlphaBlend 所需）
#include "EasyX.h"     // 包含 EasyX 头文件（IMAGE 类型定义）
#pragma comment(lib,"MSIMG32.lib")
// 声明透明绘制函数（支持缩放）
// 参数说明：
// - x, y: 目标位置（图片左上角在窗口中的坐标）
// - dst_width, dst_height: 目标尺寸（绘制到窗口后的宽度/高度，可实现缩放）
// - img: 要绘制的 IMAGE 对象指针（需已加载包含 Alpha 通道的 PNG 图片）
inline void putimage_alpha(int x, int y, int dst_width, int dst_height, IMAGE* img) {
    if (!img) return;  // 图片为空时直接返回

    // 获取源图片的设备上下文（DC）和原始尺寸
    HDC src_dc = GetImageHDC(img);  // 源图片 DC
    int src_width = img->getwidth(); // 源图片原始宽度
    int src_height = img->getheight();// 源图片原始高度

    // 获取目标窗口的设备上下文（DC）
    HDC dst_dc = GetImageHDC(NULL);  // 目标窗口 DC（当前绘图窗口）

    // 配置混合模式（Alpha 通道处理）
    BLENDFUNCTION blend = {
        AC_SRC_OVER,  // 源覆盖目标（Alpha 混合模式）
        0,            // 源透明度（0-255，0为全透明，255为不透明，此处用全局 Alpha）
        255,          // 全局 Alpha 透明度（255表示不调整）
        AC_SRC_ALPHA  // 启用源图片的 Alpha 通道
    };

    // 调用 Windows API 实现 Alpha 混合绘制
    AlphaBlend(
        dst_dc,       // 目标 DC
        x, y,         // 目标位置
        dst_width,    // 目标宽度（缩放后的宽度）
        dst_height,   // 目标高度（缩放后的高度）
        src_dc,       // 源 DC
        0, 0,         // 源图片起始位置（从左上角开始）
        src_width,    // 源图片原始宽度（用于读取像素）
        src_height,   // 源图片原始高度（用于读取像素）
        blend         // 混合模式配置
    );
}
