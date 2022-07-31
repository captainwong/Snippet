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


static constexpr int all_type_ids[] = {
	�����������ʻ֤����������Ŀ::type_id,
	���˳��ؿ۷���Ŀ::type_id,
	�ػ����ؿ۷���Ŀ::type_id,
	���ٿ۷�::���ٿ۷ֹ�����Ŀ::type_id,
	������ؿ۷���Ŀ::type_id,
	Σ��Ʒ������ؿ۷���Ŀ::type_id,
	�����۷���Ŀ::type_id,
};

static constexpr bool all_type_id_mutual_exclusive(const int* ids, const size_t len) {
	auto begin = ids;
	for (size_t i = 0; i < len; i++) {
		for (size_t j = i + 1; j < len; j++) {
			if (*(begin + i) == *(begin + j)) {
				return false;
			}
		}
	}
	return true;
}

static_assert(
	all_type_id_mutual_exclusive(all_type_ids, sizeof(all_type_ids) / sizeof(all_type_ids[0])), "��typeid��ͬ����һ���к�"
);


class �ݿ� : jlib::noncopyable {
protected:
	static constexpr auto path = "�ݿ�.dat";
	std::unordered_map<int, std::map<int, ��ĿbasePtr>> all��Ŀ{};
	double target_ratio = 0.1; // ������ϰʱ�������ʵ���ratio������ϰ
	static constexpr double min_ratio = 0.001;
	static constexpr double max_ratio = 0.5;

	��ĿbasePtr create��Ŀ(int type_id) {
		switch (type_id) {
		case �����������ʻ֤����������Ŀ::type_id: return std::make_shared<�����������ʻ֤����������Ŀ>();
		case ���˳��ؿ۷���Ŀ::type_id:return std::make_shared<���˳��ؿ۷���Ŀ>();
		case �ػ����ؿ۷���Ŀ::type_id:return std::make_shared<�ػ����ؿ۷���Ŀ>();
		case ���ٿ۷�::���ٿ۷ֹ�����Ŀ::type_id:return std::make_shared<���ٿ۷�::���ٿ۷ֹ�����Ŀ>();
		case ������ؿ۷���Ŀ::type_id:return std::make_shared<������ؿ۷���Ŀ>();
		case Σ��Ʒ������ؿ۷���Ŀ::type_id:return std::make_shared<Σ��Ʒ������ؿ۷���Ŀ>();
		case �����۷���Ŀ::type_id:return std::make_shared<�����۷���Ŀ>();
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

	bool load(std::unordered_map<int, std::map<int, ��ĿbasePtr>>& ps) {
		FILE* f = fopen(path, "rb");
		do {
			ps.clear();
			if (!f) { break; }
			read_element(target_ratio);
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
			return true;
		} while (0);
		if (f) { fclose(f); }
		ps.clear();
		return false;
	}

	bool save(const std::unordered_map<int, std::map<int, ��ĿbasePtr>>& ps) {
		FILE* f = fopen(path, "wb");
		do {
			if (!f) { break; }
			write_element(target_ratio);
			size_t sz = count��Ŀ(ps);
			write_element(sz);
			for (const auto& i : ps) {
				for (const auto& j : i.second) {
					write_element(i.first);
					if (!j.second->write(f)) break;
				}
			}

		} while (0);
		if (f) { fclose(f); }
		return false;
	}

	std::unordered_map<int, std::map<int, ��ĿbasePtr>> getAll��Ŀ() {
		std::unordered_map<int, std::map<int, ��ĿbasePtr>> map;

		/*{
			int baseline = 0;
			for (const auto& i : g_�����۷ֹ���) {
				if (i.���� == NULL) {
					baseline = i.id;
				} else {
					map[�����۷���Ŀ::type_id][i.id - baseline] =
						(std::make_shared<�����۷���Ŀ>(�����۷���Ŀ{ i.id - baseline, i.����, i.�۷� }));
				}
			}
		}*/
#define LOAD_�����۷�(��Ŀ) \
{ \
	int baseline = 0; \
	for (const auto& i : g_##��Ŀ##����) { \
		if (i.���� == NULL) { \
		baseline = i.id; \
		} else { \
			map[��Ŀ##��Ŀ::type_id][i.id - baseline] = \
				(std::make_shared<��Ŀ##��Ŀ>(��Ŀ##��Ŀ{ i.id - baseline, i.����, i.�۷� })); \
		} \
	} \
}

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
			int baseline = 0;
			int count = 0;
			for (const auto& i : g_���˳��ؿ۷ֹ���) {
				if ((int)i.����.size() > count) {
					count = (int)i.����.size();
				}
			}
			for (const auto& i : g_���˳��ؿ۷ֹ���) {
				if (i.����.empty()) {
					baseline = i.id;
				} else {
					for (const auto& j : i.����) {
						int id = (i.id - baseline) * count + j;
						map[���˳��ؿ۷���Ŀ::type_id][id] =
							(std::make_shared<���˳��ؿ۷���Ŀ>(���˳��ؿ۷���Ŀ{ id, j, i.��Χ, i.�۷� }));
					}
				}
			}
		}

		{
			int baseline = 0;
			for (const auto& i : g_�ػ����ؿ۷ֹ���) {
				if (i.���� == NULL) {
					baseline = i.id;
				} else {
					map[�ػ����ؿ۷���Ŀ::type_id][i.id - baseline] =
						(std::make_shared<�ػ����ؿ۷���Ŀ>(�ػ����ؿ۷���Ŀ{ i.id - baseline, i.����, i.�۷� }));
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

		LOAD_�����۷�(������ؿ۷�);
		LOAD_�����۷�(Σ��Ʒ������ؿ۷�);
		LOAD_�����۷�(�����۷�);
		

		return map;
	}

	void learn_׼�ݳ��ͼ�����() {
		printf("%s\n", ׼�ݳ��ͼ�����2string(rand() % 2).c_str());
	}

	void learn_�����������ʻ֤��������() {
		do_test(all��Ŀ[�����������ʻ֤����������Ŀ::type_id]);
	}

	void learn_���˳��ؿ۷ֹ���() {
		do_test(all��Ŀ[���˳��ؿ۷���Ŀ::type_id]);
	}

	void learn_�ػ����ؿ۷ֹ���() {
		do_test(all��Ŀ[�ػ����ؿ۷���Ŀ::type_id]);
	}

	void learn_���ٿ۷ֹ���() {
		do_test(all��Ŀ[���ٿ۷�::���ٿ۷ֹ�����Ŀ::type_id]);
	}

	void learn_������ؿ۷ֹ���() {
		do_test(all��Ŀ[������ؿ۷���Ŀ::type_id]);
	}

	void learn_Σ��Ʒ������ؿ۷ֹ���() {
		do_test(all��Ŀ[Σ��Ʒ������ؿ۷���Ŀ::type_id]);
	}

	void learn_�����۷ֹ���() {
		do_test(all��Ŀ[�����۷���Ŀ::type_id]);
	}

	void join(std::map<int, ��ĿbasePtr>& origin, const std::map<int, ��ĿbasePtr>& other) {
		for (const auto& i : other) {
			auto iter = origin.find(i.first);
			if (iter == origin.end()) {
				origin[i.first] = i.second;
			} else {
				bool inserted = false;
				for (int j = 0; j < INT_MAX; j++) {
					iter = origin.find(j);
					if (iter == origin.end()) {
						origin[j] = i.second;
						inserted = true;
						break;
					}
				}
				if (!inserted) {
					printf("kill me %s:%d\n", __FILE__, __LINE__);
					exit(1);
				}
			}
		}
	}

	std::map<int, ��ĿbasePtr> filter(const std::map<int, ��ĿbasePtr>& origin) {
		std::map<int, ��ĿbasePtr> dst;
		for (const auto& i : origin) {
			if (i.second->stat.ratio() >= target_ratio) {
				dst[i.first] = i.second;
			}
		}
		return dst;
	}

	void learn_���п۷ֹ���() {
		std::map<int, ��ĿbasePtr> qs;
		for (auto type_id : all_type_ids) {
			if (type_id != �����������ʻ֤����������Ŀ::type_id) {
				join(qs, all��Ŀ[type_id]);
			}
		}
		do_test(qs);
	}

	void learn_������ϰ() {
		std::map<int, ��ĿbasePtr> qs;
		for (auto type_id : all_type_ids) {
			join(qs, filter(all��Ŀ[type_id]));
		}
		if (qs.empty()) {
			printf("�����¼Ϊ�գ�\n");
			return;
		}
		do_test(qs);
	}

	void learn_����Ŀ�������() {
		system("cls");
		printf("��ǰĿ�������Ϊ%2.2f%%�������ʵ��ڴ�ֵ����ϰ����ʱ���ٳ��֡�\n", target_ratio * 100.0);
		double ratio = 0.0;
		printf("�������µ�Ŀ������ʣ�");
		scanf("%lf", &ratio);
		if (ratio / 100.0 < min_ratio || ratio / 100.0 > max_ratio) {
			printf(RED("������Ӧ������ %.2lf%% ~ %.2lf%% ֮�䡣\n"), min_ratio * 100.0, max_ratio * 100.0);
			system("pause");
			return;
		}

		target_ratio = ratio;
		printf(GREEN("�ѽ�Ŀ��������޸�Ϊ %2.2lf��"), target_ratio);
		system("pause");
	}

	void learn_�鿴ͳ����Ϣ() {
		system("cls");
		//size_t total_ans_times = 0;
		//size_t total_incorrect_times = 0;

		struct item {
			double ratio;
			int type_id;
			int id;
		};

		uint64_t total_correct_times = 0;
		std::list<item> ids;
		for (const auto& i : all��Ŀ) {
			for (const auto& j : i.second) {
				if (j.second->stat.total_incorrect_times > 0) {
					double ratio = j.second->stat.ratio();
					if (ids.empty()) {
						ids.emplace_back(item({ ratio, i.first, j.first }));
					} else {
						bool inserted = false;
						for (auto begin = ids.begin(); begin != ids.end(); begin++) {
							if (begin->ratio < ratio) {
								ids.insert(begin, item({ ratio, i.first, j.first }));
								inserted = true;
								break;
							}
						}
						if (!inserted) {
							ids.emplace_back(item({ ratio, i.first, j.first }));
						}
					}
				} else {
					total_correct_times += j.second->stat.total_correct_times;
				}
			}
		}

		if (total_correct_times == 0) {
			printf("�����¼Ϊ�գ�\n");
		} else if (ids.empty()) {
			printf("��ϲ�㣬һ�����%llu���⣬һ�ⲻ��ȫ����ȷ��\n", total_correct_times);
		} else {
			printf("һ����Թ�%llu���⣬����%u���⣬���ⰴ�մ���������Ϊ��\n", total_correct_times, ids.size());
			for (const auto& i : ids) {
				const auto& q = all��Ŀ[i.type_id][i.id];
				printf("������%2.2f%%����Ŀ��%s\n��ȷ��Ϊ��%s\n\n", i.ratio * 100.0, q->question().c_str(), q->answer().c_str());
			}
		}
	}

	void learn_���ͳ����Ϣ() {
		system("cls");
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
	void run() {
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
		append(learn_�ػ����ؿ۷ֹ���);
		append(learn_���ٿ۷ֹ���);
		append(learn_������ؿ۷ֹ���);
		append(learn_Σ��Ʒ������ؿ۷ֹ���);
		append(learn_�����۷ֹ���);
		append(learn_���п۷ֹ���);
		append(learn_������ϰ);
		append(learn_����Ŀ�������);
		append(learn_�鿴ͳ����Ϣ);
		append(learn_���ͳ����Ϣ);

#undef append

		time_t start = time(NULL);
		srand((unsigned int)start);

		all��Ŀ = getAll��Ŀ();

		// ��ȡͳ����Ϣ
		std::unordered_map<int, std::map<int, ��ĿbasePtr>> fs;
		load(fs);
		for (const auto& i : fs) {
			for (const auto& j : i.second) {
				all��Ŀ[i.first][j.first]->stat = j.second->stat;
			}
		}

		while (1) {
			int n = 0, sr = 0;
			do {
				system("cls");
				printf("��ǰ����ѡ�����Ŀ�У�\n");
				for (size_t i = 0; i < ѧϰ��Ŀ.size(); i++) {
					const auto& ��Ŀ = ѧϰ��Ŀ[i];
					printf("  %2d��%s\n", i + 1, ��Ŀ.name.c_str());
				}
				printf("  %2u: �˳�\n", ѧϰ��Ŀ.size() + 1);
				printf("��ѡ��Ҫѧϰ����Ŀ��");
				sr = scanf("%d", &n);
				if (sr == 0) { break; }
			} while (n < 1 || n > (int)ѧϰ��Ŀ.size() + 1);
			if (sr == 0) { break; }
			if (n == (int)ѧϰ��Ŀ.size() + 1) {
				break;
			}
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