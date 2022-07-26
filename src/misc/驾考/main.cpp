#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <time.h>
#include <Windows.h>
#include <jlib/base/noncopyable.h>

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


class �ݿ� : jlib::noncopyable {
protected:
	static constexpr auto path = "�ݿ�.dat";
	std::unordered_map<int, std::map<int, ��ĿbasePtr>> all��Ŀ{};

	��ĿbasePtr create��Ŀ(int type_id) {
		switch (type_id) {
		case �����������ʻ֤����������Ŀ::type_id: return std::make_shared<�����������ʻ֤����������Ŀ>();
		case ���˳��ؿ۷�::�����˿۷ֹ�����Ŀ::type_id:return std::make_shared<���˳��ؿ۷�::�����˿۷ֹ�����Ŀ>();
		case ���ٿ۷�::���ٿ۷ֹ�����Ŀ::type_id:return std::make_shared<���ٿ۷�::���ٿ۷ֹ�����Ŀ>();
		case �����۷�::�����۷���Ŀ::type_id:return std::make_shared<�����۷�::�����۷���Ŀ>();
		default: return nullptr;
		}
	}

	size_t count��Ŀ(const std::unordered_map<int, std::map<int, ��ĿbasePtr>>& ps) {
		size_t count = 0;
		for (const auto& i : ps) {
			count += i.second.size();
		}
		return count;
	}

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
		FILE* f = fopen(path, "wb");
		do {
			if (!f) { break; }
			size_t sz = count��Ŀ(ps);
			write_elment(sz);
			for (const auto& i : ps) {
				for (const auto& j : i.second) {
					write_elment(i.first);
					if (!j.second->write(f)) break;
				}
			}

		} while (0);
		if (f) { fclose(f); }
		return false;
	}

	std::unordered_map<int, std::map<int, ��ĿbasePtr>> getAll��Ŀ() {
		std::unordered_map<int, std::map<int, ��ĿbasePtr>> map;

		{
			int baseline = 0;
			int count = 0;
			for (const auto& i : g_�����������ʻ֤��������) {
				if ((int)i.����.size() > count) {
					count = (int)i.����.size();
				}
			}
			for (const auto& i : g_�����������ʻ֤��������) {
				if (i.����.empty()) {
					baseline = i.id;
				} else {
					for (size_t j = 0; j < i.����.size(); j++) {
						int id = (i.id - baseline) * count + j;
						auto ptr = (std::make_shared<�����������ʻ֤����������Ŀ>(�����������ʻ֤����������Ŀ{ id, i, j }));
						int tid = ptr->type_id;
						map[�����������ʻ֤����������Ŀ::type_id][id] = ptr;
					}
				}
			}

		}

		{
			using namespace ���˳��ؿ۷�;
			int baseline = 0;
			int count = 0;
			for (const auto& i : g_�����˿۷ֹ���) {
				if ((int)i.����.size() > count) {
					count = (int)i.����.size();
				}
			}
			for (const auto& i : g_�����˿۷ֹ���) {
				if (i.����.empty()) {
					baseline = i.id;
				} else {
					for (const auto& j : i.����) {
						int id = (i.id - baseline) * count + j;
						map[�����˿۷ֹ�����Ŀ::type_id][id] =
							(std::make_shared<�����˿۷ֹ�����Ŀ>(�����˿۷ֹ�����Ŀ{ id, j, i.��Χ, i.�۷� }));
					}
				}
			}
		}

		{
			using namespace ���ٿ۷�;
			int baseline = 0;
			int count = 0;
			for (const auto& i : g_���ٿ۷ֹ���) {
				if ((int)i.����.size() > count) {
					count = (int)i.����.size();
				}
			}
			for (const auto& i : g_���ٿ۷ֹ���) {
				if (i.����.empty()) {
					baseline = i.id;
				} else {
					for (const auto& j : i.����) {
						int id = (i.id - baseline) * count + j;
						map[���ٿ۷ֹ�����Ŀ::type_id][id] =
							(std::make_shared<���ٿ۷ֹ�����Ŀ>(���ٿ۷ֹ�����Ŀ{ id, j, i.��·, i.��Χ, i.�۷� }));
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
		do_test(all��Ŀ[�����������ʻ֤����������Ŀ::type_id]);
	}

	void learn_���˳��ؿ۷ֹ���()
	{
		do_test(all��Ŀ[���˳��ؿ۷�::�����˿۷ֹ�����Ŀ::type_id]);
	}

	void learn_���ٿ۷ֹ���()
	{
		do_test(all��Ŀ[���ٿ۷�::���ٿ۷ֹ�����Ŀ::type_id]);
	}

	void learn_�����۷ֹ���() {
		do_test(all��Ŀ[�����۷�::�����۷���Ŀ::type_id]);
	}

	void learn_�鿴ͳ����Ϣ() {
		//size_t total_ans_times = 0;
		//size_t total_incorrect_times = 0;

		std::map<std::pair<int, int>, double> ids;
		for (const auto& i : all��Ŀ) {
			for (const auto& j : i.second) {
				if (j.second->stat.total_incorrect_times > 0) {
					double ratio = j.second->stat.total_incorrect_times * 100.0 / (j.second->stat.total_correct_times + j.second->stat.total_incorrect_times);
					if (ids.empty()) {
						ids[std::make_pair<>(i.first, j.first)] = ratio;
					} else {
						for (auto begin = ids.begin(); begin != ids.end(); begin++) {
							if (begin->second < ratio) {
								ids.insert(begin, std::make_pair<>(std::make_pair<>(i.first, j.first), ratio));
								break;
							}
						}
					}
				}

			}
		}

		if (ids.empty()) {
			printf("�����¼Ϊ�գ�\n");
		} else {
			printf("һ������%u���⣬���ⰴ�մ���������Ϊ��\n", ids.size());
			for (const auto& i : ids) {
				const auto& q = all��Ŀ[i.first.first][i.first.second];
				printf("������%2.2f����Ŀ��%s\n��ȷ��Ϊ��%s\n\n", i.second, q->question().c_str(), q->answer().c_str());
			}
		}
	}

	void learn_���ͳ����Ϣ() {
		int yes = 0;
		printf(RED("ȷ��Ҫ���ͳ����Ϣ�𣿴˲����޷��ָ���\n"));
		printf("����1������������������ֵȡ����");
		scanf("%d", &yes);
		if (yes != 1) return;
		for (auto& i : all��Ŀ) {
			for (auto& j : i.second) {
				j.second->stat.clear();
			}
		}
		save(all��Ŀ);
		printf("�ѳɹ����ͳ����Ϣ��\n");
	}

public:
	void run()
	{
		struct ѧϰ��Ŀ {
			std::string name;
			std::function<void(�ݿ�*)> func;
		};

		//ѧϰ��Ŀ ddd{ "", std::bind(&��Ŀmanager::learn_�����������ʻ֤��������, std::placeholders::_1) };
		std::vector<ѧϰ��Ŀ> ѧϰ��Ŀ;
#define append(func) ѧϰ��Ŀ.push_back(struct ѧϰ��Ŀ({#func, std::bind(&�ݿ�::func, std::placeholders::_1)})); ѧϰ��Ŀ.back().name = ѧϰ��Ŀ.back().name.substr(6);
		append(learn_׼�ݳ��ͼ�����);
		append(learn_�����������ʻ֤��������);
		append(learn_���˳��ؿ۷ֹ���);
		append(learn_���ٿ۷ֹ���);
		append(learn_�����۷ֹ���);
		append(learn_�鿴ͳ����Ϣ);
		append(learn_���ͳ����Ϣ);

#undef append

		time_t start = time(NULL);
		srand((unsigned int)start);

		all��Ŀ = getAll��Ŀ();

		// ��ȡͳ����Ϣ
		auto fs = load();
		for (const auto& i : fs) {
			for (const auto& j : i.second) {
				all��Ŀ[i.first][j.first]->stat = j.second->stat;
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

			ѧϰ��Ŀ[n - 1].func(this);

			printf("�Ƿ����ѧϰ����1���������������˳�\n");
			scanf("%d", &n);
			if (n != 1) { break; }
		}

		// ����ͳ����Ϣ
		save(all��Ŀ);

		printf("�ټ���\n");
	}
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

	�ݿ�().run();
}