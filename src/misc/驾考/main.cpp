#define _CRT_SECURE_NO_WARNINGS

#include <functional>
#include <time.h>
#include <Windows.h>

#include "׼�ݳ���.h"
#include "���˳��ؿ۷�.h"
#include "���ٿ۷�.h"



void learn_׼�ݳ��ͼ�����()
{
	printf("%s\n", ׼�ݳ��ͼ�����2string(rand() % 2).c_str());
}

void learn_�����������ʻ֤��������()
{
	std::vector<�����������ʻ֤����������Ŀ> ��Ŀ�б�;
	for (const auto& i : g_�����������ʻ֤��������) {
		for(size_t j = 0; j < i.���복��.size(); j++) {
			��Ŀ�б�.push_back(�����������ʻ֤����������Ŀ({ i, j }));
		}
	}

	do_test(��Ŀ�б�);
}

void learn_���˳��ؿ۷ֹ���()
{
	using namespace ���˳��ؿ۷�;
	std::vector<�����˿۷ֹ�����Ŀ> ��Ŀ�б�;
	for (const auto& i : g_�����˿۷ֹ���) {
		for (const auto& j : i.����) {
			��Ŀ�б�.push_back(�����˿۷ֹ�����Ŀ({ j, i.��Χ, i.�۷� }));
		}
	}

	do_test(��Ŀ�б�);
}

void learn_���ٿ۷ֹ���()
{
	using namespace ���ٿ۷�;
	std::vector<���ٿ۷ֹ�����Ŀ> ��Ŀ�б�;
	for (const auto& i : g_���ٿ۷ֹ���) {
		for (const auto& j : i.����) {
			��Ŀ�б�.push_back(���ٿ۷ֹ�����Ŀ({ j, i.��·, i.��Χ, i.�۷� }));
		}
	}

	do_test(��Ŀ�б�);
}

struct ѧϰ��Ŀ {
	std::string name;
	std::function<void(void)> func;
};

int main()
{
	// https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console
	system("reg add HKEY_CURRENT_USER\\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f");

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 32;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"����");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	std::vector<ѧϰ��Ŀ> ѧϰ��Ŀ;
#define append(func) ѧϰ��Ŀ.push_back(struct ѧϰ��Ŀ({#func, &func})); ѧϰ��Ŀ.back().name = ѧϰ��Ŀ.back().name.substr(6);

	append(learn_׼�ݳ��ͼ�����);
	append(learn_�����������ʻ֤��������);
	append(learn_���˳��ؿ۷ֹ���);
	append(learn_���ٿ۷ֹ���);

#undef append

	srand((unsigned int)time(NULL));
	int n = 0;
	do {
		printf("��ǰ����ѡ�����Ŀ�У�\n");
		for (size_t i = 0; i < ѧϰ��Ŀ.size(); i++) {
			const auto& ��Ŀ = ѧϰ��Ŀ[i];
			printf("  %d��%s\n", i + 1, ��Ŀ.name.c_str());
		}
		printf("��ѡ��Ҫѧϰ����Ŀ��");
		scanf("%d", &n);
	} while (n < 1 || n > (int)ѧϰ��Ŀ.size());

	ѧϰ��Ŀ[n - 1].func();
}