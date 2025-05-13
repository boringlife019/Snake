#pragma once

#include <Windows.h>  // ����� Windows API ͷ�ļ���AlphaBlend ���裩
#pragma comment(lib, "gdi32.lib")  // ������ gdi32.lib �⣨AlphaBlend ���裩
#include "EasyX.h"     // ���� EasyX ͷ�ļ���IMAGE ���Ͷ��壩
#pragma comment(lib,"MSIMG32.lib")
// ����͸�����ƺ�����֧�����ţ�
// ����˵����
// - x, y: Ŀ��λ�ã�ͼƬ���Ͻ��ڴ����е����꣩
// - dst_width, dst_height: Ŀ��ߴ磨���Ƶ����ں�Ŀ��/�߶ȣ���ʵ�����ţ�
// - img: Ҫ���Ƶ� IMAGE ����ָ�루���Ѽ��ذ��� Alpha ͨ���� PNG ͼƬ��
inline void putimage_alpha(int x, int y, int dst_width, int dst_height, IMAGE* img) {
    if (!img) return;  // ͼƬΪ��ʱֱ�ӷ���

    // ��ȡԴͼƬ���豸�����ģ�DC����ԭʼ�ߴ�
    HDC src_dc = GetImageHDC(img);  // ԴͼƬ DC
    int src_width = img->getwidth(); // ԴͼƬԭʼ���
    int src_height = img->getheight();// ԴͼƬԭʼ�߶�

    // ��ȡĿ�괰�ڵ��豸�����ģ�DC��
    HDC dst_dc = GetImageHDC(NULL);  // Ŀ�괰�� DC����ǰ��ͼ���ڣ�

    // ���û��ģʽ��Alpha ͨ������
    BLENDFUNCTION blend = {
        AC_SRC_OVER,  // Դ����Ŀ�꣨Alpha ���ģʽ��
        0,            // Դ͸���ȣ�0-255��0Ϊȫ͸����255Ϊ��͸�����˴���ȫ�� Alpha��
        255,          // ȫ�� Alpha ͸���ȣ�255��ʾ��������
        AC_SRC_ALPHA  // ����ԴͼƬ�� Alpha ͨ��
    };

    // ���� Windows API ʵ�� Alpha ��ϻ���
    AlphaBlend(
        dst_dc,       // Ŀ�� DC
        x, y,         // Ŀ��λ��
        dst_width,    // Ŀ���ȣ����ź�Ŀ�ȣ�
        dst_height,   // Ŀ��߶ȣ����ź�ĸ߶ȣ�
        src_dc,       // Դ DC
        0, 0,         // ԴͼƬ��ʼλ�ã������Ͻǿ�ʼ��
        src_width,    // ԴͼƬԭʼ��ȣ����ڶ�ȡ���أ�
        src_height,   // ԴͼƬԭʼ�߶ȣ����ڶ�ȡ���أ�
        blend         // ���ģʽ����
    );
}
