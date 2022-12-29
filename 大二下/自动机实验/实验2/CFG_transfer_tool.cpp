#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define TERMINAL 0
#define NONTERMINAL 1

#define GENERATING_START_CHAR_ERROR 1
#define GENERATING_FOTMAT_ERROR 2
#define GENERATING_CHAR_OVER_SET 3
#define GENERATING_START_CHAR_EXIST 4

#define KEEP_MODE 1
#define CHANGE_MODE 2

#define GET_BYTE(vbyte, index) (((vbyte) & (1LLU << ((index) ^ 63))) != 0)
#define SET_BYTE(vbyte, index) ((vbyte) |= (1LLU << ((index) ^ 63)))
#define CLR_BYTE(vbyte, index) ((vbyte) &= (~(1LLU << ((index) ^ 63))))

//�������ݽṹ
typedef struct SET{
	long long state;				//����Ԫ��״̬
	int len;						//����Ԫ����Ŀ
}SET;

//����ʽ���ݽṹ
typedef struct GENERATING {
	char start_char;				//����ʽ��ʼ��
	char** next_char;				//����ʽ�ַ�������
	int len;						//����ʽ��Ŀ
}GENERATING;

//CFG���ݽṹ
typedef struct CFG {
	SET terminal_set;							//�ս������
	SET nonterminal_set;						//���ս������
	GENERATING* generating;						//����ʽ����
	char start_char;							//��ʼ��
	int generating_len;							//����ʽ��Ŀ
}CFG;

//�������ã���ָ���ַ�ӳ�䵽һ������0-63������
static int char_range(char c) {

	if ((c >= 'a' && c <= 'z'))
		return c - 'a';
	else if(c >= '0'&&c <= '9')
		return 26 + c - '0';
	else if(c >= 'A' && c <= 'Z')
		return 36 + c - 'A';
	else if(c == '+')
		return 62;
	else if(c == '*')
		return 63;
	else
		return -1;
}

//�������ã�char_range()�ķ�ӳ��
static char invert_char_range(int index) {
	if(index >= 0&&index <= 25)
		return index+'a';
	else if(index >= 26&&index <= 35)
		return index-26+'0';
	else if(index >= 36&&index <= 61)
		return index-36+'A';
	else if(index == 62)
		return '+';
	else if(index == 63)
		return '*';
	else 
		return 'A';
}

//�������ã���ʼ�����ϣ����ڴ����û����룩
static int init_set(SET* set,int mode){
	int i;
    set->state = 0;
	int len = 0;
	char s[50] = {0};
	if(mode == 0)printf("Enter information of terminal set (lengh of set, set)\n");
	else if(mode == 1)printf("Enter information of nonterminal set (lengh of set, set)\n");
	scanf_s("%d",&len);
	for (i = 0; ; i++) {
		char ch = getchar();
		if (ch == ',' || ch == ' ') {
			i--;
			continue;
		}
		if(ch == '\n') 
			break;
		int m = char_range(ch);
		if (m >= 0 && m <= 35 && !mode) {
			if (!GET_BYTE(set->state,m)) {
				SET_BYTE(set->state,m);
			}
			else {
				while((ch = getchar()) != '\n');
				printf("Input error! You entered the same element\n");
				return 1;
			}
		}
		else if(!mode){
			while((ch = getchar()) != '\n');
			printf("Input error! You should input lowercase letters or number\n");
			return 1;
		}
		if (m >= 36 && m < 62 && mode) {
			if (!GET_BYTE(set->state,m)) {
				SET_BYTE(set->state,m);
			}
			else {
				while((ch = getchar()) != '\n');
				printf("Input error! You entered the same element\n");
				return 1;
			}
		}
		else if(mode){
			while((ch = getchar()) != '\n');
			printf("Input error! You should input uppercase letters\n");
			return 1;
		}
	}

	if (i != len) {
		printf("Input error! The number of characters you entered does not match the actual number of characters you entered\n");
		return 1;
	}
	set->len = len;
	return 0;
}

//�������ã�������set��Ӧ��Ԫ����Ϣ��ӡ����
static void show_set(SET set) {
	printf("{");
	for (int i = 0,j = 0; i < 64; i++) {
		if (GET_BYTE(set.state,i)) {
			if(j != 0) printf(",");
			if(i == 62) printf("S0");
			else printf("%c",invert_char_range(i));
			j++;
		}
	}
	printf("}\n");
}

//�������ã���ʼ������ʽ�����ڴ����û����룩
static int init_generating(GENERATING* g,long long state,long long generating_start_char_state) {
	int len = 0;
	int next_char_count = 1;
	char ch,ch1,ch2;
	char string[100];
	g->len = 0;
	scanf_s("\n");
	scanf_s("%c",&(g->start_char),1);
	if (char_range(g->start_char) <= 35 || char_range(g->start_char) >= 62) {
		while((ch = getchar()) != '\n');
		return GENERATING_START_CHAR_ERROR;
	}
	if (GET_BYTE(generating_start_char_state, char_range(g->start_char))) {
		while((ch = getchar()) != '\n');
		return GENERATING_START_CHAR_EXIST;
	}	
	ch1 = getchar();
	ch2 = getchar();
	ch = getchar();
	if (ch1 != '-' || ch2 != '>' || ch == '|') {
		while(getchar() != '\n');
		return GENERATING_FOTMAT_ERROR;
	}
	while (ch != '\n') {
		if(!GET_BYTE(state,char_range(ch)) && ch != '|')
			if (ch != '*') {
				while(getchar() != '\n');
				return GENERATING_CHAR_OVER_SET;
			}
		if (ch == '|' && string[len - 1] == '|') {
			while(getchar() != '\n');
			return GENERATING_FOTMAT_ERROR;
		}
		if(ch == '|')
			next_char_count++;
		string[len] = ch;
		len++;
		ch = getchar();
	}
	string[len] = 0;
	g->len = next_char_count;
	if (next_char_count == 1) {
		g->next_char = (char**)calloc(next_char_count,sizeof(char*));
		if (g->next_char == NULL) {
			printf("Single Gennerating creation error!");
			exit(0);
		}
		g->next_char[0] = (char*)calloc(len+1,sizeof(char));
		if (g->next_char[0] == NULL) {
			printf("Single Gennerating creation error!");
			exit(0);
		}
		memcpy(g->next_char[0],string,len);
		g->next_char[0][len] = 0;
	}
	else {
		g->next_char = (char**)calloc(next_char_count,sizeof(char*));
		if (g->next_char == NULL) {
			printf("Single Gennerating creation error!");
			exit(0);
		}

		for (int i = 0,j = 0,count = 0; i <= len; i++) {
			if (string[i] == '|'||string[i] == 0) {
				int g_len = i-j;
				g->next_char[count] = (char*)calloc(g_len + 1,sizeof(char));
				if (g->next_char[0] == NULL) {
					printf("Single Gennerating creation error!");
					exit(0);
				}
				memcpy(g->next_char[count],string + j,g_len);
				g->next_char[count][g_len] = 0;
				count ++;
				j = i + 1;
			}
		}
	}
	return 0;
}

//�������ã���ʼ��CFG����ʽ���ϣ����ڴ����û����룩
static void init_CFG_generating(CFG* cfg) {
	long long generating_start_char_state ;
	while (1) {
		printf("Enter the kind of generating of different nonterminal char\n");
		scanf_s("\n");
		scanf_s("%d",(&cfg->generating_len));
		if (cfg->generating_len > cfg->nonterminal_set.len) {
			printf("Input error!The number is larger than the length of nonterminal set\n");
			printf("Input again plz\n");
		}
		else if (cfg->generating_len <= 0) {
			printf("Input error!The number is non positive number\n");
			printf("Input again plz\n");
		}
		else
			break;
	}

	cfg->generating = (GENERATING*)calloc(cfg->generating_len,sizeof(GENERATING));
	if (cfg->generating == NULL) {
		printf("Gennerating creation error!\n");
		exit(0);
	}
	generating_start_char_state = 0;
	long long mix_state = cfg->nonterminal_set.state | cfg->terminal_set.state;
	printf("Enter information of generating (ep: A->b|s|B )\n");
	for (int i = 0; i < cfg->generating_len; i++) {
		int flag = init_generating(&cfg->generating[i],mix_state,generating_start_char_state);
		switch (flag)
		{
		case GENERATING_START_CHAR_ERROR:
			printf("Input Error! Wrong start character entered\n");
			i--;
			printf("Please input the right format again\n");
			scanf_s("\n");
			break;
		
		case GENERATING_FOTMAT_ERROR:
			printf("Input Error! Wrong gengerating format enterer\n");
			i--;
			printf("Please input the right format again\n");
			scanf_s("\n");
			break;

		case GENERATING_CHAR_OVER_SET:
			printf("Input Error! This generating has the char which isn't in the terminal set or nonterminal set\n");
			i--;
			printf("Please input the right format again\n");
			scanf_s("\n");
			break;
		
		case GENERATING_START_CHAR_EXIST:
			printf("Input Error! The start char of generating has been used\n");
			i--;
			printf("Please input the right format again\n");
			scanf_s("\n");
			break;
		}
		if(flag == 0)
			SET_BYTE(generating_start_char_state,char_range(cfg->generating[i].start_char));
	}
}

//�������ã���ʼ��CFG�����ڴ����û����룩
static void init_CFG(CFG* cfg) {
	char ch;
	int flag = 0;
	printf("Please enter information of CFG\n");
	flag = init_set(&(cfg->nonterminal_set), NONTERMINAL);
	while (flag) {
		printf("Input again plz\n");
		flag = init_set(&(cfg->nonterminal_set), NONTERMINAL);
	}
	flag = init_set(&(cfg->terminal_set), TERMINAL);
	while (flag) {
		printf("Input again plz\n");
		flag = init_set(&(cfg->terminal_set), TERMINAL);
	}
	printf("Please enter the start char of CFG\n");
	scanf_s("\n");
	scanf_s("%c",&ch,1);
	while (char_range(ch) == -1 || !GET_BYTE(cfg->nonterminal_set.state, char_range(ch))) {
		if (char_range(ch) == -1) {
			while(getchar() != '\n');
			printf("Input Error!Start char over the union set\n");
			printf("Please enter the start char of CFG again\n");
		}
		else {
			while(getchar() != '\n');
			printf("Input Error!Start char isn't in the nonterminal set\n");
			printf("Please enter the start char of CFG again\n");
		}
		scanf_s("%c",&ch,1);
	}
	cfg->start_char = ch;
	init_CFG_generating(cfg);
}
static void show_generating(GENERATING g) {
	if(g.start_char != '+')printf("%c->", g.start_char);
	else printf("S0->");
	for (int i = 0; i < g.len; i++) {
		printf("%s",g.next_char[i]);
		if(i != g.len - 1) printf("|");
		else printf("\n");
	}
}

//�������ã���ӡCFG����Ϣ
static void show_CFG(CFG cfg) {
	printf("Nonterminal Set:\n");
	show_set(cfg.nonterminal_set);
	printf("Terminal Set:\n");
	show_set(cfg.terminal_set);
	printf("Start char:\n");
	if(cfg.start_char == '+')
		printf("S0\n");
	else printf("%c\n",cfg.start_char);
	printf("Generating:\n");
	for (int i = 0; i < cfg.generating_len; i++) {
		show_generating(cfg.generating[i]);
	}
}

//�������ã����ݼ���״̬set_state��
static long long find_nonterminal_set(CFG cfg, long long set_state) {
	long long set = 0;
	for (int i = 0; i < cfg.generating_len; i++) {
		GENERATING g = cfg.generating[i];
		for (int j = 0; j < cfg.generating[i].len; j++) {
			int m = 0;	
			char ch = g.next_char[j][m];
			while (ch != 0) {
				if(!GET_BYTE(set_state,char_range(ch)))
					break;
				m++;
				ch = cfg.generating[i].next_char[j][m];
			}
			if (ch == 0) {
				SET_BYTE(set,char_range(cfg.generating[i].start_char));
			}
		}
	}
	return set;
}

//��״̬set_state����Ϣת��������set
static void state_to_set(SET *set, long long set_state) {
	int num = 0;
	for (int i = 0; i < 64; i++) {
		if(GET_BYTE(set_state,i))
			num ++;
	}
	set->state = set_state;
	set->len = num;
}

//�������ã�alg_1()���Ӻ������ڴ����㷨�еĵݹ鲿��
static void sub_alg_1(CFG cfg, long long* set1_state,long long* set2_state, long long set_state) {
	if (*set1_state != *set2_state) {
		*set1_state = *set2_state;
		long long set = *set1_state;
		set |= set_state;
		set = find_nonterminal_set(cfg,set);
		*set2_state = *set1_state | set;
		sub_alg_1(cfg,set1_state,set2_state,set_state);
	}
}

//�������ã���̿����е��㷨1����һ�£����ڻ�õ���ŵķ��ս������
static void alg_1(CFG cfg, long long set_state,SET *set) {
	long long set1_state = 0;
	long long set2_state = 0;
	set2_state = find_nonterminal_set(cfg,set_state);
	sub_alg_1(cfg,&set1_state,&set2_state,set_state);
	set->state = set2_state;
}

//�������ã����ڸ��Ƽ�����Ϣ��KEEP_MODE����ȫ���Ƽ��ϣ�CHANGE_MODE�»����һ��S0Ԫ�ء�
static void cfg_copy_set(SET set, SET* d_set, int mode) {
	if (mode == KEEP_MODE) {
		int set_len = set.len;
		d_set->len = set_len;
		d_set->state = set.state;
	}
	else if (mode == CHANGE_MODE) {
		int set_len = set.len;
		d_set->len = set_len + 1;
		d_set->state = 0;
		SET_BYTE(d_set->state,char_range('+'));
		d_set->state |= set.state;
	}
}

//�������ã�no_epsilon_generate()���Ӻ����������滻����ʽ�еĵ���ŵķ��ս��
static void no_epsilon_string(char s1[][40], int* index, char* s2, long long state) {
	char s_1[40];
	char s_2[40];
	char s[40];
	int s_index[40] = {0};
	int buf_dif[200] = {0};
	char buf[200][40] ;
	int buf_index = 0;
	int m = 0,n1 = 0,n2 = 0,count = 1;
	while (s2[m] != 0) {
	if (GET_BYTE(state, char_range(s2[m]))) {
		s_1[n1++] = s2[m];
		s_index[m] = 1;
	}
	else {
		s_2[n2++] = s2[m];
		s_index[m] = 2;
	}
		m++;
	}
	s_1[n1] = 0;
	s_2[n2] = 0;
	int s_1_len = strlen(s_1);
	int s_2_len = strlen(s_2);
	for (int i = 0; i < s_1_len; i++) 
		count *= 2;
	for (int i = 0; i < count; i++) {
		int i_1 = i, i1 = 0, i2 = 0, is = 0;
		for (int j = 0; j < m; j++) {
			if (s_index[j] == 1) {
				if (i_1 % 2 == 1 && s_1[i1] != '*')
					s[is++] = s_1[i1];
				i1++;
				i_1 /= 2;
			}
			else s[is++] = s_2[i2++];
		}
		s[is] = 0;
		int len = strlen(s);
		if (len != 0) {
			memcpy(buf[buf_index], s, len);
			buf[buf_index][len] = 0;
			(buf_index)++;
		}
	}
	if (buf_index > 0) {
		buf_dif[buf_index - 1] = 1;
		for (int i = 0; i < buf_index - 1; i++) {
			if (buf_dif[i] == 0) {
				for (int j = i + 1; j < buf_index; j++) {
					if (strcmp(buf[i], buf[j]) == 0) {
						buf_dif[j] = -1;
					}
				}
				buf_dif[i] = 1;
			}
		}
		for (int i = 0; i < buf_index; i++) {
			if (buf_dif[i] != 1)
				continue;
			int str_len = strlen(buf[i]);
			memcpy(s1[*index], buf[i], str_len);
			s1[*index][str_len] = 0;
			(*index)++;
		}
	}
}

//�������ã������滻����ʽ�еĵ���ŵķ��ս��
static int no_epsilon_generate(GENERATING g, GENERATING* no_e_g, int mode, long long state) {
	char s[400][40];
	int index = 0;
	if (mode == KEEP_MODE) {
		for (int i = 0; i < g.len; i++) {
			int m = 0;
			while (g.next_char[i][m]) {
				if (GET_BYTE(state, char_range(g.next_char[i][m])))
					break;
				m++;
			}
			if (g.next_char[i][m] == 0) {
				int len = strlen(g.next_char[i]);
				memcpy(s[index], g.next_char[i],len);
				s[index][len] = 0;
				index ++;
			}
			else {
				no_epsilon_string(s, &index, g.next_char[i], state);
			}
		}
		if(index == 0)
			return 0;
		else {
			no_e_g->start_char = g.start_char;
			no_e_g->next_char = (char**)calloc(index, sizeof(char*));
			if (no_e_g->next_char == NULL) {
				printf("Char** creation error!\n");
				exit(0);
			}
			no_e_g->len = index;
			for (int i = 0; i < index; i++) {
				int len = strlen(s[i]);
				no_e_g->next_char[i] = (char*)calloc(len + 1, sizeof(char));
				if (no_e_g->next_char[i] == NULL) {
					printf("Char* creation error!\n");
					exit(0);
				}
				memcpy(no_e_g->next_char[i], s[i], len);
				no_e_g->next_char[i][len] = 0;
			}
			return 1;
		}		
	}
	else if (mode == CHANGE_MODE) {
		s[index][0] = g.start_char, s[index++][1] = 0;
		s[index][0] = '*', s[index++][1] = 0;
		no_e_g->start_char = '+';
		no_e_g->len = index;
		no_e_g->next_char = (char**)calloc(index, sizeof(char*));
		if (no_e_g->next_char == NULL) {
			printf("Char** creation error!\n");
			exit(0);
		}
		for (int i = 0; i < index; i++) {
			int len = strlen(s[i]);
			no_e_g->next_char[i] = (char*)calloc(len + 1, sizeof(char));
			if (no_e_g->next_char[i] == NULL) {
				printf("Char* creation error!\n");
				exit(0);
			}
			memcpy(no_e_g->next_char[i], s[i], len);
			no_e_g->next_char[i][len] = 0;
		}
		return 1;
	}
	return 1;
}

//�������ã���֪ת��Ϊ�ަŲ���ʽ��CFG�в���ʽ��Ŀ
static int no_epsilon_cfg_len(CFG cfg, long long state) {
	int count = 0;
	for (int i = 0; i < cfg.generating_len; i++) {
		if(GET_BYTE(state,char_range(cfg.generating[i].start_char)))
			if (cfg.generating[i].len == 1&&cfg.generating[i].next_char[0][0] == '*') {
					continue;
			}
		count ++;
	}
	return count;
}

//�������ã���cfgת�����ަŲ���ʽCFG�洢��no_e_cfg��
static void no_epsilon_cfg(CFG cfg,CFG* no_e_cfg) {
	SET set;
	int j = 0;
	long long state;
	long long e_state = 0;
	SET_BYTE(e_state,char_range('*'));
	alg_1(cfg,e_state,&set);
	state = set.state;
	SET_BYTE(state,char_range('*'));
	cfg_copy_set(cfg.terminal_set,&no_e_cfg->terminal_set,KEEP_MODE);
	int count = no_epsilon_cfg_len(cfg,set.state);
	if (GET_BYTE(set.state, char_range(cfg.start_char))) {
		no_e_cfg->generating = (GENERATING*)calloc(++count,sizeof(GENERATING));
		no_e_cfg->generating_len = count;
		if (no_e_cfg->generating == NULL) {
			printf("Generating creation error!\n");
			exit(0);
		}
		cfg_copy_set(cfg.nonterminal_set,&no_e_cfg->nonterminal_set,CHANGE_MODE);
		no_e_cfg->start_char = '+';
		for (int i = 0; i < cfg.generating_len; i++) {
			if(cfg.generating[i].start_char == cfg.start_char)
				no_epsilon_generate(cfg.generating[i],&no_e_cfg->generating[j++],CHANGE_MODE,state);
				int m = no_epsilon_generate(cfg.generating[i],&no_e_cfg->generating[j],KEEP_MODE,state);
				if(m == 1&&j != count-1) j++;
			else {
				int m = no_epsilon_generate(cfg.generating[i],&no_e_cfg->generating[j],KEEP_MODE,state);
				if(m == 1 && j != count -1) j++;
			}		
		}
	}
	else {
		no_e_cfg->generating = (GENERATING*)calloc(count,sizeof(GENERATING));
		if (no_e_cfg->generating == NULL) {
			printf("Generating creation error!\n");
			exit(0);
		}
		
		no_e_cfg->start_char = cfg.start_char;
		no_e_cfg->generating_len = count;
		cfg_copy_set(cfg.nonterminal_set,&(no_e_cfg->nonterminal_set),KEEP_MODE);

		for (int i = 0; i < cfg.generating_len; i++) {
			int m = no_epsilon_generate(cfg.generating[i],&no_e_cfg->generating[j],KEEP_MODE,state);
			if(m == 1 && j != count - 1) j++;
		}
	}
	show_CFG(*no_e_cfg);
}

//�������ã�sub_alg_2()���Ӻ���������CFG�е������ַ�
static long long find_use_char(CFG cfg, long long state) {
	long long new_state = 0;
	long long t = cfg.nonterminal_set.state;
	for (int i = 0; i < cfg.generating_len; i++) {
		if (GET_BYTE(state, char_range(cfg.generating[i].start_char))) {
			GENERATING g = cfg.generating[i];
			for (int j = 0; j < g.len; j++) {
				int m = 0;
				while (g.next_char[j][m] != 0) {
					int n = char_range(g.next_char[j][m]);
					if(GET_BYTE(t,n))
						SET_BYTE(new_state,n);
					m++;
				}
			}
		}
	}
	return state | new_state;
}

//�������ã�alg_2()���Ӻ��������ڸ����㷨�ĵݹ����
static void sub_alg_2(CFG cfg, long long* state_1, long long* state_2) {
	if (*state_1 != *state_2) {
		*state_1 = *state_2;
		*state_2 = find_use_char(cfg,*state_1);
		sub_alg_2(cfg,state_1,state_2);
	}
}

//�������ã������ϵ��㷨2��Ŀ��һ�£��ҳ�CFG�е����÷���
static void alg_2(CFG cfg, SET* set) {
	long long begin_state = 0;
	long long state = 0;
	SET_BYTE(begin_state, char_range(cfg.start_char));
	state = find_use_char(cfg, begin_state);
	sub_alg_2(cfg,&begin_state,&state);
	state_to_set(set,state);
}

//�������ã�����CFG���ݽṹ���õ��˶�̬�ڴ棬�����װһ�����������ͷ���Ƭ�ڴ�
static void free_CFG(CFG* cfg) {
	for (int i = 0; i < cfg->generating_len; i++) {
		for (int j = 0; j < cfg->generating[i].len; j++) {
			free(cfg->generating[i].next_char[j]);
		}
	}
	free(cfg->generating);
}

//�������ã���cfg�������÷��ŵ�CFG���洢��target_cfg
static int remove_useless_char(CFG cfg, CFG* target_cfg) {
	long long t_state = cfg.terminal_set.state;
	long long nt_state = cfg.nonterminal_set.state;
	long long unach_state = 0;
	SET set1;
	CFG cfg1;
	int count = 0;
	alg_1(cfg,t_state,&set1);
	unach_state  = nt_state ^ set1.state; 
	if (GET_BYTE(unach_state, char_range(cfg.start_char))) {
		printf("The start char can't achieve\n");
		return 0;
	}
	for (int i = 0; i < cfg.generating_len; i++) {
		char n = cfg.generating[i].start_char;
		if(!GET_BYTE(unach_state,char_range(n)))
			count ++;
	}
	cfg1.start_char = cfg.start_char;
	cfg_copy_set(set1,&cfg1.nonterminal_set,KEEP_MODE);
	cfg_copy_set(cfg.terminal_set,&cfg1.terminal_set,KEEP_MODE);
	cfg1.generating_len = count;
	cfg1.generating = (GENERATING*)calloc(count,sizeof(GENERATING));
	if (cfg1.generating == NULL) {
		printf("Generating creation error!\n");
		exit(0);
	}
	for (int i = 0,ch_index = 0; i < cfg.generating_len; i++) {
		int index[40] = { 0 };
		int n = 0;
		GENERATING g = cfg.generating[i];
		if(GET_BYTE(unach_state,g.start_char))
			continue;
		for (int j = 0; j < g.len; j++) {
			int m = 0;
			while (g.next_char[j][m] != 0) {
				if (GET_BYTE(unach_state, char_range(g.next_char[j][m]))) {
					index[j] = 1;
					break;
				}
				m++;
			}
			if(g.next_char[j][m] == 0) n++;
		}
		if (n > 0) {
			cfg1.generating[ch_index].start_char = g.start_char;
			cfg1.generating[ch_index].len = n;
			cfg1.generating[ch_index].next_char  = (char**)calloc(n,sizeof(char*));
			if (cfg1.generating[ch_index].next_char == NULL) {
				printf("Char** creation error!\n");
				exit(0);
			}
			for (int j = 0,n_index = 0; j < g.len; j++) {
				if(index[j] == 1)continue;
				int len = strlen(g.next_char[j]);
				cfg1.generating[ch_index].next_char[n_index] = (char*)calloc(len+1,sizeof(char));
				if (cfg1.generating[ch_index].next_char[n_index] == NULL) {
					printf("Char* creation error!\n");
					exit(0);
				}
				memcpy(cfg1.generating[ch_index].next_char[n_index],g.next_char[j],len);
				cfg1.generating[ch_index].next_char[n_index][len] = 0;
				if(n_index != n -1)
					n_index ++;
			}
			if(ch_index != count-1)ch_index++;
		}
	}
	long long nustate = 0;
	SET set2;
	alg_2(cfg1,&set2);
	nustate = set2.state ^ cfg1.nonterminal_set.state;
	if (GET_BYTE(nustate, char_range(cfg1.start_char))) {
		printf("The start char can't achieve\n");
		return 0;
	}
	count = 0;
	for (int i = 0; i < cfg1.generating_len; i++) {
		char n = cfg1.generating[i].start_char;
		if (!GET_BYTE(nustate, char_range(n))) {
			count ++;
		}
	}
	target_cfg->start_char = cfg.start_char;
	cfg_copy_set(set2,&target_cfg->nonterminal_set,KEEP_MODE);
	cfg_copy_set(cfg1.terminal_set,&target_cfg->terminal_set,KEEP_MODE);
	target_cfg->generating_len = count;
	target_cfg->generating = (GENERATING*)calloc(count,sizeof(GENERATING));
	if (target_cfg->generating == NULL) {
		printf("Generating creation error!\n");
		exit(0);
	}
	for (int i = 0,ch_index = 0; i < cfg1.generating_len; i++) {
		int index[40] = { 0 };
		int n = 0;
		GENERATING g = cfg1.generating[i];
		if(GET_BYTE(nustate,char_range(g.start_char)))
			continue;
		for (int j = 0; j < g.len; j++) {
			int m = 0;
			while (g.next_char[j][m] != 0) {
				if (GET_BYTE(nustate, char_range(g.next_char[j][m]))) {
					index[j] = 1;
					break;
				}
				m++;
			}
			if(g.next_char[j][m] == 0) n++;
		}
		if (n > 0) {
			target_cfg->generating[ch_index].start_char = g.start_char;
			target_cfg->generating[ch_index].len = n;
			target_cfg->generating[ch_index].next_char  = (char**)calloc(n,sizeof(char*));
			if (target_cfg->generating[ch_index].next_char == NULL) {
				printf("Char** creation error!\n");
				exit(0);
			}
			for (int j = 0,n_index = 0; j < g.len; j++) {
				if(index[j] == 1)continue;
				int len = strlen(g.next_char[j]);
				target_cfg->generating[ch_index].next_char[n_index] = (char*)calloc(len+1,sizeof(char));
				if (target_cfg->generating[ch_index].next_char[n_index] == NULL) {
					printf("Char* creation error!\n");
					exit(0);
				}
				memcpy(target_cfg->generating[ch_index].next_char[n_index],g.next_char[j],len);
				target_cfg->generating[ch_index].next_char[n_index][len] = 0;
				if(n_index != n -1)
					n_index ++;
			}
			if(ch_index != count-1)ch_index++;
		}
	}
	show_CFG(*target_cfg);
	free_CFG(&cfg1);
	return 1;
}

//�������ã�alg_3()���Ӻ��������ڸ����㷨��ѭ������
static long long sub_alg_3(CFG cfg, long long state1) {
	long long state = 0;
	for (int i = 0; i < cfg.generating_len; i++) {
		GENERATING g = cfg.generating[i];
		if(!GET_BYTE(state1,char_range(g.start_char)))
			continue;
		for (int j = 0; j < g.len; j++) {
			if (strlen(g.next_char[j]) == 1) {
				if (GET_BYTE(cfg.nonterminal_set.state, char_range(g.next_char[j][0]))) {
					SET_BYTE(state,char_range(g.next_char[j][0]));
				}
			}
		}
	}
	return state | state1;
}

//�������ã����һ�����ս����Ӧ�ĵ�Ԫż�Լ�����Ϣ
static long long alg_3(CFG cfg, long long state) {
	long long state2 = state;
	state= sub_alg_3(cfg,state2);
	if (state2 != state) {
		return alg_3(cfg,state);
	}
	else {
		return state;
	}
}

//�������ã� no_single_generate()���Ӻ�������CFG�е�һ������ʽ�еĵ�Ԫż���滻��
static void no_single_str(CFG cfg, char ch, char buf[][40], int* index,long long state) {
	char s[200][40];
	int ntflag[100] = {0};
	int flag[200] = {0};
	s[0][0] = ch;
	s[0][1] = 0;
	int index_1 = 1;
	long long state1 = state;
	while (1) {
		for (int i = 0; i < index_1; i++) {
			if (flag[i] != 0)
				continue;
			else {
				char c = s[i][0];
				int len = strlen(s[i]);
				if (len == 1 && GET_BYTE(state, char_range(c))) {
					if (GET_BYTE(state, char_range(c)) && !GET_BYTE(state1, char_range(c))) {
					flag[i] = -1;
					continue;
					}
					else {
						CLR_BYTE(state1,char_range(c));
					}
					flag[i] = -1;
					int j = 0;
					for(; j < cfg.generating_len; j++) {
						if(c == cfg.generating[j].start_char)
							break;
					}
					GENERATING g = cfg.generating[j];
					for (j = 0; j < g.len; j++) {
						len = strlen(g.next_char[j]);
						memcpy(s[index_1],g.next_char[j],len);
						s[index_1][len] = 0;
						index_1++;
					}
				}else 
					flag[i] = 1;
			}
		}
		int m = 0;
		for (; m < index_1; m++) {
			if(flag[m] == 0)
				break;
		}
		if(m == index_1 )
			break;
	}
	for (int i = 0; i < index_1 - 1; i++) {
		if (flag[i] == 1) {
			for (int j = i + 1; j < index_1; j++) {
				if(strcmp(s[i],s[j]) == 0)
					flag[j] = -1;
			}
		}
	}
	for (int i = 0; i < index_1; i++) {
		if (flag[i] == 1) {
			int len = strlen(s[i]);
			memcpy(buf[*index],s[i],len);
			buf[*index][len] = 0;
			(*index)++;
		}
	}
}

//�������ã��滻����ʽ�еĵ�Ԫż��
static int no_single_generate(CFG cfg, GENERATING g, GENERATING* g1,long long state) {
	char buf[200][40];
	int flag[200] = {0};
	int count = 0;
	int index = 0;
	int index_dif = 0;
	for (int i = 0; i < g.len; i++) {
		long long state1 = state;
		if (strlen(g.next_char[i]) == 1 && GET_BYTE(state1, char_range(g.next_char[i][0]))) {
			no_single_str(cfg,g.next_char[i][0],buf,&index,state1);
		}
		else {
			int len = strlen(g.next_char[i]);
			memcpy(buf[index],g.next_char[i],len);
			buf[index++][len] = 0;
		}
	}
	count = 0;
	for (int i = 0; i < index ; i++) {
		if (flag[i] == 0) {
			for (int j = i + 1; j < index; j++) {
				if(strcmp(buf[i],buf[j]) == 0)
					flag[j] = -1;
			}
			count ++;
		}
	}
	g1->next_char = (char**)calloc(count,sizeof(char*));
	g1->start_char = g.start_char;
	g1->len = count;
	if (g1->next_char == NULL) {
		printf("char** creation error!\n");
		exit(0);
	}
	for (int i = 0,j = 0; i < index; i++) {
		if (flag[i] == 0) {
			int len = strlen(buf[i]);
			g1->next_char[j] = (char*)calloc(len + 1,sizeof(char));
			if (g1->next_char[j] == NULL) {
				printf("char* creation error!\n");
				exit(0);
			}
			memcpy(g1->next_char[j],buf[i],len);
			g1->next_char[j][len] = 0;
			if(j < count -1) j++;
		}
	}
}

//�������ã���cfgת��Ϊ�޵�Ԫż��CFG���洢��cfg1��
static void no_single_cfg(CFG cfg,CFG* cfg1) {
	long long set_state[30];
	long long state = 0;
	for (int i = 0; i < cfg.generating_len; i++) {
		GENERATING g = cfg.generating[i];
		state = 0;
		SET_BYTE(state,char_range(g.start_char));
		set_state[i] = alg_3(cfg,state);
	}
	cfg1->generating_len = cfg.generating_len;
	cfg1->start_char = cfg.start_char;
	cfg_copy_set(cfg.nonterminal_set,&cfg1->nonterminal_set,KEEP_MODE);
	cfg_copy_set(cfg.terminal_set,&cfg1->terminal_set,KEEP_MODE);
	cfg1->generating = (GENERATING*)calloc(cfg.generating_len,sizeof(GENERATING));
	if (cfg1->generating == NULL) {
		printf("genrating creation error!\n");
		exit(0);
	}
	for (int i = 0; i < cfg.generating_len; i++) {
		no_single_generate(cfg,cfg.generating[i],&cfg1->generating[i],set_state[i]);
	}
	show_CFG(*cfg1);
}

int main() {
	printf("===========================�Զ���ʵ���===========================\n");
	printf("=========================CFG�ȼ��ķ�ת��==========================\n");
	printf("==================================================================\n");
	while(1){
	CFG cfg,cfg1,cfg2,cfg3;
	init_CFG(&cfg);
	printf("\n=========================�������CFG============================\n");
	show_CFG(cfg);
	printf("\n==========================�ަ�����ʽ============================\n");
	no_epsilon_cfg(cfg,&cfg1);
	printf("\n==========================�޵�����ʽ============================\n");
	no_single_cfg(cfg1,&cfg2);
	printf("\n=========================û�������ַ�===========================\n");
	int m = remove_useless_char(cfg2,&cfg3);
	printf("\n===========================ת�����=============================\n");
	free_CFG(&cfg);
	free_CFG(&cfg1);
	free_CFG(&cfg2);
	if(m == 1)free_CFG(&cfg3);
	printf("����q�����˳��������������ַ��������½���ת��\n");
	char ch = getchar();
	if(ch == 'q')
		break;
	}
	return 0;
}