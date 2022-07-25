#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX


#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <time.h>
#include <Windows.h>

#include "׼�ݳ���.h"
#include "�۷�.h"


constexpr bool all_type_id_mutual_exclusive(std::initializer_list<int> id) {
	auto begin = id.begin();
	for (size_t i = 0; i < id.size(); i++) {
		for (size_t j = i + 1; j < id.size(); j++) {
			if (*(begin + i) == *(begin + j)) {
				return false;
			}
		}
	}
	return true;
}

static_assert(
	all_type_id_mutual_exclusive({ 
		�����������ʻ֤����������Ŀ::type_id,
		���˳��ؿ۷�::�����˿۷ֹ�����Ŀ::type_id,
		���ٿ۷�::���ٿ۷ֹ�����Ŀ::type_id,
		�����۷�::�����۷���Ŀ::type_id,
	}), "��typeid��ͬ����һ���к�"
);

size_t typeSize(int type_id) {
	switch (type_id) {
	case �����������ʻ֤����������Ŀ::type_id: return sizeof(�����������ʻ֤����������Ŀ);
	case ���˳��ؿ۷�::�����˿۷ֹ�����Ŀ::type_id:return sizeof(���˳��ؿ۷�::�����˿۷ֹ�����Ŀ);
	case ���ٿ۷�::���ٿ۷ֹ�����Ŀ::type_id:return sizeof(���ٿ۷�::���ٿ۷ֹ�����Ŀ);
	case �����۷�::�����۷���Ŀ::type_id:return sizeof(�����۷�::�����۷���Ŀ);
	default: return 0;
	}
}

��ĿbasePtr create��Ŀ(int type_id) {
	switch (type_id) {
	case �����������ʻ֤����������Ŀ::type_id: return std::make_shared<�����������ʻ֤����������Ŀ>();
	case ���˳��ؿ۷�::�����˿۷ֹ�����Ŀ::type_id:return std::make_shared<���˳��ؿ۷�::�����˿۷ֹ�����Ŀ>();
	case ���ٿ۷�::���ٿ۷ֹ�����Ŀ::type_id:return std::make_shared<���ٿ۷�::���ٿ۷ֹ�����Ŀ>();
	case �����۷�::�����۷���Ŀ::type_id:return std::make_shared<�����۷�::�����۷���Ŀ>();
	default: return nullptr;
	}
}

struct ��Ŀmanager {
	static constexpr auto path = "�ݿ�.dat";

	std::unordered_map<int, std::map<int, ��ĿbasePtr>> load() {
		std::unordered_map<int, std::map<int, ��ĿbasePtr>> ps;
		FILE* f = fopen(path, "rb");
		do {
			if (!f) { break; }
			int count = 0;
			if (fread(&count, 1, sizeof(count), f) != sizeof(count)) {
				break;
			}
			bool failed = false;
			for (int i = 0; i < count; i++) {
				int type_id = 0;
				if (fread(&type_id, 1, sizeof(type_id), f) != sizeof(type_id)) {
					failed = true;
					break;
				}
				auto ptr = create��Ŀ(type_id);
				if (!ptr) { failed = true; break; }
				if (!ptr->read(f)) { failed = true; break; }
				ps[type_id][ptr->id] = ptr;
			}
			if (failed) break;

			fclose(f);
			return ps;
		} while (0);
		if (f) { fclose(f); }
		return std::unordered_map<int, std::map<int, ��ĿbasePtr>>();
	}

	bool save(const std::unordered_map<int, std::map<int, ��ĿbasePtr>>& ps) {
		FILE* f = fopen(path, "rb");
		do {
			if (!f) { break; }
			size_t sz = ps.size();
			write_elment(sz);
			for (const auto& i : ps) {
				for (const auto& j : i.second) {
					if (!j.second->write(f)) break;
				}
			}

		} while (0);
		if (f) { fclose(f); }
		return false;
	}
};

std::unordered_map<int, std::map<int, ��ĿbasePtr>> g_��Ŀ{};

std::unordered_map<int, std::map<int, ��ĿbasePtr>> getAll��Ŀ() {
	std::unordered_map<int, std::map<int, ��ĿbasePtr>> map;

	{
		int baseline = 0;
		for (const auto& i : g_�����������ʻ֤��������) {
			if (i.���복��.empty()) {
				baseline = i.id;
			} else {
				for (size_t j = 0; j < i.���복��.size(); j++) {
					map[�����������ʻ֤����������Ŀ::type_id][i.id - baseline] = 
						(std::make_shared<�����������ʻ֤����������Ŀ>(�����������ʻ֤����������Ŀ{ i.id - baseline, i, j }));
				}
			}
		}

	}

	{
		using namespace ���˳��ؿ۷�;
		int baseline = 0;
		for (const auto& i : g_�����˿۷ֹ���) {
			if (i.����.empty()) {
				baseline = i.id;
			} else {
				for (const auto& j : i.����) {
					map[�����˿۷ֹ�����Ŀ::type_id][i.id - baseline] =
						(std::make_shared<�����˿۷ֹ�����Ŀ>(�����˿۷ֹ�����Ŀ{ i.id - baseline, j, i.��Χ, i.�۷� }));
				}
			}
		}
	}

	{
		using namespace ���ٿ۷�;
		int baseline = 0;
		for (const auto& i : g_���ٿ۷ֹ���) {
			if (i.����.empty()) {
				baseline = i.id;
			} else {
				for (const auto& j : i.����) {
					map[���ٿ۷ֹ�����Ŀ::type_id][i.id - baseline] =
						(std::make_shared<���ٿ۷ֹ�����Ŀ>(���ٿ۷ֹ�����Ŀ{ i.id - baseline, j, i.��·, i.��Χ, i.�۷� }));
				}
			}
		}
	}

	{
		using namespace �����۷�;
		int baseline = 0;
		for (const auto& i : g_�����۷ֹ���) {
			if (i.���� == NULL) {
				baseline = i.id;
			} else {
				map[�����۷���Ŀ::type_id][i.id - baseline] =
					(std::make_shared<�����۷���Ŀ>(�����۷���Ŀ{ i.id - baseline, i.����, i.�۷� }));
			}
		}
	}

	return map;
}

void learn_׼�ݳ��ͼ�����()
{
	printf("%s\n", ׼�ݳ��ͼ�����2string(rand() % 2).c_str());
}

void learn_�����������ʻ֤��������()
{
	do_test(g_��Ŀ[�����������ʻ֤����������Ŀ::type_id]);
}

void learn_���˳��ؿ۷ֹ���()
{
	do_test(g_��Ŀ[���˳��ؿ۷�::�����˿۷ֹ�����Ŀ::type_id]);
}

void learn_���ٿ۷ֹ���()
{
	do_test(g_��Ŀ[���ٿ۷�::���ٿ۷ֹ�����Ŀ::type_id]);
}

void learn_�����۷ֹ���() {
	do_test(g_��Ŀ[�����۷�::�����۷���Ŀ::type_id]);
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
	append(learn_�����۷ֹ���);

#undef append

	time_t start = time(NULL);
	srand((unsigned int)start);

	g_��Ŀ = getAll��Ŀ();
	��Ŀmanager mgr;

	// ��ȡ
	auto fs = mgr.load();
	for (const auto& i : fs) {
		for (const auto& j : i.second) {
			g_��Ŀ[i.first][j.first] = j.second;
		}
	}

	while (1) {
		int n = 0;
		do {
			system("cls");
			printf("��ǰ����ѡ�����Ŀ�У�\n");
			for (size_t i = 0; i < ѧϰ��Ŀ.size(); i++) {
				const auto& ��Ŀ = ѧϰ��Ŀ[i];
				printf("  %d��%s\n", i + 1, ��Ŀ.name.c_str());
			}
			printf("��ѡ��Ҫѧϰ����Ŀ��");
			scanf("%d", &n);
		} while (n < 1 || n >(int)ѧϰ��Ŀ.size());

		ѧϰ��Ŀ[n - 1].func();

		printf("�Ƿ����ѧϰ����1���������������˳�\n");
		scanf("%d", &n);
		if (n != 1) { break; }
	}

	printf("�ټ���\n");
}