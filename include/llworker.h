#ifndef _PLLWORKER_H_
#define _PLLWORKER_H_

#define LLWORKER_OBL_OPTION 1

#include <vector>
#include <cstddef>

class llWorker {

 protected:

	 const char *command_name;
	 int  command_index;
	 int  checked_value, checked_pos;
	 bool repeat_worker;

	 std::vector<const char*> name;
	 std::vector<int*>    flag;
	 std::vector<int>     flag_cache;
	 std::vector<int*>    i_value;
	 std::vector<float*>  f_value;
	 std::vector<double*> d_value;
	 std::vector<char**>  s_value;
	 std::vector<char*>   i_value_cache;
	 std::vector<char*>   f_value_cache;
	 std::vector<char*>   d_value_cache;
	 std::vector<char*>   s_value_cache;
	 std::vector<int>     i_value_num;
	 std::vector<int>     f_value_num;
	 std::vector<int>     d_value_num;
	 std::vector<int>     s_value_num;
	 std::vector<int>     opt;
	 std::vector<int>     used;

	 void AddElement(void) {
		 int size = GetSize();
		 name.resize(size + 1);
		 name[size] = NULL;
		 flag.resize(size + 1);
		 flag[size] = NULL;
		 flag_cache.resize(size + 1);
		 flag_cache[size] = 0;
		 i_value.resize(size + 1);
		 i_value[size] = NULL;
		 f_value.resize(size + 1);
		 f_value[size] = NULL;
		 d_value.resize(size + 1);
		 d_value[size] = NULL;
		 s_value.resize(size + 1);
		 s_value[size] = NULL;
		 i_value_cache.resize(size + 1);
		 i_value_cache[size] = NULL;
		 f_value_cache.resize(size + 1);
		 f_value_cache[size] = NULL;
		 d_value_cache.resize(size + 1);
		 d_value_cache[size] = NULL;
		 s_value_cache.resize(size + 1);
		 s_value_cache[size] = NULL;
		 i_value_num.resize(size + 1);
		 i_value_num[size]   = 0;
		 f_value_num.resize(size + 1);
		 f_value_num[size]   = 0;
		 d_value_num.resize(size + 1);
		 d_value_num[size]   = 0;
		 s_value_num.resize(size + 1);
		 s_value_num[size]   = 0;
		 opt.resize(size + 1);
		 opt[size] = 0;
		 used.resize(size + 1);
	 };

 public:

    llWorker();

	void SetCommandName(const char *_name) {
		command_name = _name;
	};

	const char *GetCommandName(void) {
		return command_name;
	}

    int RegisterFlag (const char *_name, int    *_flag,   int _opt = 0);
	int RegisterValue(const char *_name, int    *_value,  int _opt = 0);
	int RegisterValue(const char *_name, float  *_value,  int _opt = 0);
	int RegisterValue(const char *_name, double *_value,  int _opt = 0);
	int RegisterValue(const char *_name, char   **_value, int _opt = 0);

	int GetSize(void) {
		return name.size();
	}

	virtual llWorker * Clone() {
		return new llWorker(*this);
	}

	int CheckFlag (char *_flag);
	int CheckValue(char *_value);
	int AddValue  (char *_value);
	int ReplaceFlags(void);

	int SetValue (char *_name, char *_value) {
		if (!CheckValue(_name)) return 0;
		if (!AddValue(_value))  return 0;
		return 1;
	};

	int Used(const char *_flag);

	virtual int RegisterOptions(void);
	virtual int Prepare(void);
	virtual int Exec(void);

	void SetCommandIndex(int _com) {
		command_index = _com;
	}

	int GetCommandIndex() {
		return command_index;
	}

	int Repeat() {
		return repeat_worker;
	}

	virtual int IsRepeatWorker() {
		return 0;
	}

	virtual void Print(void);

};

#endif
