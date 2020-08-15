#include<iostream>
#include<conio.h>
#include<string.h>
#include<cstring>
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include"mylib.h"
#define MAXMB 300
#define MAXDAY 10
#define MAXDONG 20
#define Enter 13
#define Space 32
#define ESC 27
#define Backspace 8
#define Tab 9
#define Ctrl_s 19
#define Delete 224+83
#define Insert 224+82
#define Up 224+72
#define Down 224+80
#define Left 224+75
#define Right 224+77
#define Black 0
#define Blue 1
#define Green 2
#define Aqua 3
#define Red 4
#define White 15
#define cot 45 //55 font 20
#define dong 11
#define KhoangCach 2 //dat ve cach nhau 2 tieng

using namespace std;

//khai bao
// danh sach may bay
struct MayBay {
	char soHieu[15];
	char loai[40];
	int soDay;
	int soDong;
};

struct listMB {
	int n;
	MayBay* nodes[MAXMB];
};

// danh sach ve
struct Ve {
	char soVe[4];
	char CMND[12];
};

struct listVe {
	int n; //so luong ve da dat
	Ve* nodes;
};

struct datetime {
	int gio;
	int phut;
	int ngay;
	int thang;
	int nam;
};

// danh sach chuyen bay
struct ChuyenBay {
	char maCB[15];
	datetime ngayGioKH;
	char sanBayDen[50];
	int trangThai; //0:huy chuyen, 1:con ve, 2:het ve, 3:hoan tat
	char soHieu[15];
	int soDay;
	int soDong;
	listVe dsVe;
};

struct nodesCB {
	ChuyenBay cb;
	struct nodesCB* next;
};
typedef struct nodesCB* nodesCB_PTR;

// danh sach hanh khach
struct HanhKhach {
	char CMND[12];
	char ho[50];
	char ten[10];
	char phai[4];
};

struct nodes_HK {
	HanhKhach hk;
	struct nodes_HK* left;
	struct nodes_HK* right;
};
typedef struct nodes_HK* nodesHK_PTR;

///////////////////////// cac nguyen mau ham ve giao dien trong mylib.h ////////////
void gotoxy(short x, short y);
int wherex(void);
int wherey(void);
void SetColor(WORD color);
void SetBGColor(WORD color);
void clrscr();

///////////////////////// cac phep toan kiem tra ngay thang/////////////////////

bool checkNamNhuan(int nam) {
	if (nam % 400 == 0) {
		return true;
	}
	else if (nam % 4 == 0 && nam % 100 != 0) {
		return true;
	}
	else return false;
}

bool checkNgayHopLeCuaThang(datetime dt) {
	int dayEndOfMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (dt.thang != 2) {
		if (dt.ngay <= dayEndOfMonth[dt.thang - 1]) {
			return true;
		}
		else return false;
	}
	else {
		if (checkNamNhuan(dt.nam)) {
			if (dt.ngay <= 29) {
				return true;
			}
			else return false;
		}
		else {
			if (dt.ngay <= dayEndOfMonth[dt.thang - 1]) {
				return true;
			}
			else return false;
		}
	}
}

bool checkNgayGioHopLe(datetime& dt) {
	if (dt.gio > 23 || dt.gio < 0) {
		return false;
	}
	if (dt.phut > 59 || dt.phut < 0) {
		return false;
	}
	if (dt.ngay <= 0 || dt.thang <= 0 || dt.thang > 12 || dt.nam <= 1900) {
		return false;
	}
	if (checkNgayHopLeCuaThang(dt) == false) {
		return false;
	}
	return true;
}

bool checkNgayLapChuyenBay(datetime dt) {
	datetime now;
	time_t tn = time(0);
	tm timenow;
	errno_t err = localtime_s(&timenow, &tn);
	if (err != 0) {
		cout << "loi lay thoi gian hien tai";
		return false;
	}
	now.gio = timenow.tm_hour;
	now.phut = timenow.tm_min;
	now.phut = timenow.tm_min;
	now.gio = timenow.tm_hour;
	now.ngay = timenow.tm_mday;
	now.thang = timenow.tm_mon + 1;
	now.nam = timenow.tm_year + 1900;

	if (dt.nam < now.nam) {
		return false;
	}
	if (dt.nam > now.nam) {
		return true;
	}
	else {
		if (dt.thang < now.thang) {
			return false;
		}
		if (dt.thang > now.thang) {
			return true;
		}
		else {
			if (dt.ngay < now.ngay) {
				return false;
			}
			if (dt.ngay > now.ngay) {
				return true;
			}
			else {
				if (dt.gio < now.gio) {
					return false;
				}
				if (dt.gio > now.gio) {
					return true;
				}
				else {
					if (dt.phut <= now.phut) {
						return false;
					}
					else {
						return true;
					}
				}
			}
		}
	}
}

bool checkNgayTrungChuyenBay(datetime dt1, datetime dt2) {
	if (dt1.gio == dt2.gio && dt1.phut == dt2.phut && dt1.ngay == dt2.ngay &&
		dt1.thang == dt2.thang && dt1.nam == dt2.nam) {
		return true;
	}
	return false;
}

bool checkThoiGian2CB(datetime dt1, datetime dt2) { // cach 2 tieng
	if (dt1.ngay == dt2.ngay && dt1.thang == dt2.thang && dt1.nam == dt2.nam) {

	}
	return true;//thoi gian hop le
}

///////////////////////// cac phep toan tren mang con tro (may bay)/////////////////

bool isEmpty(listMB ds) {
	if (ds.n == 0) {
		return true;
	}
	else return false;
}

bool isFull(listMB ds) {
	if (ds.n == MAXMB) {
		return true;
	}
	else return false;
}

void khoiTao_MB(listMB& ds) {
	ds.n = 0;
}

int insertLast_MB(listMB& ds, MayBay mb) {
	if (isFull(ds) == true) {
		return 0;
	}
	else {
		ds.nodes[ds.n] = new MayBay;
		*ds.nodes[ds.n] = mb;
		ds.n++;
		return 1;
	}
}

int searchNodes_MB(listMB ds, char* sohieu) { //tra ve vi tri tim duoc
	for (int i = 0; i < ds.n; i++) {
		if (_stricmp(ds.nodes[i]->soHieu, sohieu) == 0) {
			return i;
		}
	}
	return -1;
}

int deleteNodes_MB(listMB& ds, char* sohieu) {
	int i = searchNodes_MB(ds, sohieu);
	if (i == -1) {
		return 0;
	}
	else {
		delete ds.nodes[i]; //xoa vung nho con tro tro toi
		for (int j = i + 1; j < ds.n; j++) {
			ds.nodes[j - 1] = ds.nodes[j];
		}
		ds.n--;
		return 1;
	}
}

int deleteNodes_MB(listMB& ds, int index) { //delete khi da biet vi tri mb
	if (index == -1) {
		return 0;
	}
	else {
		delete ds.nodes[index]; //xoa vung nho con tro tro toi
		for (int j = index + 1; j < ds.n; j++) {
			ds.nodes[j - 1] = ds.nodes[j];
		}
		ds.n--;
		return 1;
	}
}

void duyetList_MB(listMB ds) {
	if (isEmpty(ds)) {
		cout << "danh sach rong";
		return;
	}
	for (int i = 0; i < ds.n; i++) {
		cout << ds.nodes[i]->soHieu << "\t" << ds.nodes[i]->loai << endl;
	}
}

void clearList_MB(listMB& ds) {
	/*for (int i = 0; i < ds.n; i++) {
		delete ds.nodes[i];
	}
	ds.n = 0;*/
	while (ds.n > 0) {
		delete ds.nodes[ds.n - 1];
		ds.n--;
	}
}

///////////////////////// cac phep toan tren mang dong (ve)//////////////////////

bool isEmpty(listVe ds) {
	if (ds.n == 0) {
		return true;
	}
	else return false;
}

bool isFull(listVe ds, int maxve) {
	if (ds.n == maxve) {
		return true;
	}
	else return false;
}

int khoiTaoDSVe(listVe& ds, int soday, int sodong) {
	if (soday <= 0 || soday > MAXDAY || sodong <= 0 || sodong > MAXDONG) {
		return 0;
	}
	ds.n = 0;
	ds.nodes = new Ve[soday * sodong];
	int k = 0;
	for (int i = 0; i < soday; i++) {
		for (int j = 1; j <= sodong; j++) {
			strcpy_s(ds.nodes[k].CMND, "trong"); //chua duoc dat
			char tmp[4] = ""; //khoi tao bien tam tmp // tmp[4]="A00"
			tmp[0] = char(i + 65);
			if (j < 10) {
				tmp[1] = '0';
				char b[2];
				_itoa_s(j, b, 10);
				//tmp[2]=b[0];
				strncat_s(tmp, b, 1);
			}
			else {
				char b[3];
				_itoa_s(j, b, 10);
				/*tmp[1] = b[0];
				tmp[2] = b[1];*/
				strncat_s(tmp, b, 2);
			}
			strcpy_s(ds.nodes[k].soVe, tmp);
			k++;
		}
	}
	return 1;
}

int searchVe(listVe ds, int soday, int sodong, char* soVe) {
	for (int i = 0; i < soday * sodong; i++) {
		if (strcmp(ds.nodes[i].soVe, soVe) == 0) {
			return i;
		}
	}
	return -1;
}

int searchVe_cmnd(listVe ds, int soday, int sodong, char* cmnd) {
	for (int i = 0; i < soday * sodong; i++) {
		if (strcmp(ds.nodes[i].CMND, cmnd) == 0) {
			return i;
		}
	}
	return -1;
}

int insertVe(listVe& ds, int soday, int sodong, Ve ve) { //dat ve
	if (isFull(ds, soday * sodong)) {
		return 0;
	}
	else {
		int i = searchVe(ds, soday, sodong, ve.soVe);
		if (i == -1) {
			return 0;
		}
		else if (strcmp(ds.nodes[i].CMND, ve.CMND) == 0) {
			cout << "khach hang chi duoc dat ve 1 lan 1 chuyen bay";
			return 0;
		}
		else {
			ds.nodes[i] = ve;
			ds.n++;
		}
	}
	return 1;
}

int deleteVe(listVe& ds, int soday, int sodong, char* cmnd) { //huy ve
	if (isEmpty(ds)) {
		return 0;
	}
	else {
		int i = searchVe_cmnd(ds, soday, sodong, cmnd);
		if (i == -1) {
			cout << "khong tim thay ve co so cmnd tren";
			return 0;
		}
		else {
			strcpy_s(ds.nodes[i].CMND, "trong");
			ds.n--;
		}
	}
	return 1;
}

void clearList_Ve(listVe& ds) {
	delete[]ds.nodes;
}

//////////////// cac phep toan tren danh sach lien ket don (chuyen bay)//////////

bool isEmpty(nodesCB_PTR first) {
	if (first == NULL) {
		return true;
	}
	else return false;
}

void khoiTaoDS_CB(nodesCB_PTR& first) {
	first = NULL;
}

void insertFirst_CB(nodesCB_PTR& first, ChuyenBay cb) {
	nodesCB_PTR p = new nodesCB;
	p->cb = cb;
	p->next = first;
	first = p;
}

void insertLast_CB(nodesCB_PTR& first, ChuyenBay cb) {
	nodesCB_PTR p = new nodesCB;
	p->cb = cb;
	p->next = NULL;
	if (first == NULL) {
		first = p;
	}
	else {
		nodesCB_PTR last;
		for (last = first; last->next != NULL; last = last->next);
		last->next = p;
	}
}

int insertAfter_CB(nodesCB_PTR p, ChuyenBay cb) {
	if (p == NULL) {
		return 0;
	}
	else {
		nodesCB_PTR q = new nodesCB;
		q->cb = cb;
		q->next = p->next;
		p->next = q;
	}
	return 1;
}

int insertOrder_CB(nodesCB_PTR& first, ChuyenBay cb) {
	nodesCB_PTR p, q;
	q = NULL;
	for (p = first; p != NULL && (_stricmp(p->cb.maCB, cb.maCB) < 0); p = p->next) {
		q = p;
	}
	if (q == NULL) {
		insertFirst_CB(first, cb);
		return 1;
	}
	else {
		return insertAfter_CB(q, cb);
	}
}

nodesCB_PTR searchNodes_CB(nodesCB_PTR first, char* macb) {
	for (nodesCB_PTR p = first; p != NULL; p = p->next) {
		if (_stricmp(p->cb.maCB, macb) == 0) {
			return p;
		}
	}
	return NULL;
}

nodesCB_PTR searchMBinCB(nodesCB_PTR first, char* sohieu) {
	for (nodesCB_PTR p = first; p != NULL; p = p->next) {
		if (_stricmp(p->cb.soHieu, sohieu) == 0) {
			return p;
		}
	}
	return NULL;
}

int deleteFirst_CB(nodesCB_PTR& first) {
	if (isEmpty(first)) {
		cout << "danh sach rong khong co gi de xoa";
		return 0;
	}
	else {
		nodesCB_PTR p = first;
		first = p->next;
		clearList_Ve(p->cb.dsVe); //xoa ds ve trc de giai phong vung nho
		delete p;
		return 1;
	}
}

int deleteAfter_CB(nodesCB_PTR p) {
	if (p == NULL || p->next == NULL) {
		return 0;
	}
	nodesCB_PTR q = p->next;
	p->next = q->next;
	clearList_Ve(q->cb.dsVe);
	delete q;
	return 1;
}

int deleteNodes_CB(nodesCB_PTR& first, char* macb) {
	if (isEmpty(first)) {
		cout << "danh sach rong";
		return 0;
	}
	if (strcmp(first->cb.maCB, macb) == 0) {
		return deleteFirst_CB(first);
	}
	else {
		nodesCB_PTR p;
		for (p = first; p->next != NULL && (_stricmp(p->next->cb.maCB, macb) != 0); p = p->next);
		return deleteAfter_CB(p);
	}
}

void clearList_CB(nodesCB_PTR& first) {
	nodesCB_PTR p;
	while (!isEmpty(first)) {
		p = first;
		first = first->next;
		clearList_Ve(p->cb.dsVe);
		delete p;
	}
}

void Selection_Sort(nodesCB_PTR& first) {
	nodesCB_PTR p, q, pmin;
	ChuyenBay min;

	for (p = first; p->next != NULL; p = p->next) {
		min = p->cb;
		pmin = p;
		for (q = p->next; q != NULL; q = q->next)
			if (strcmp(q->cb.maCB, min.maCB) < 0) {
				min = q->cb;
				pmin = q;
			}
		// hoan doi truong info cua hai nut p va pmin
		pmin->cb = p->cb;
		p->cb = min;
	}
}

/////////////////// cac phep toan tren cay nhi phan tim kiem (hanh khach)////////////

void khoiTao_HK(nodesHK_PTR& root) {
	root = NULL;
}

bool isEmpty(nodesHK_PTR root) {
	if (root == NULL) return true;
	else return false;
}

int insert_Node_HK(nodesHK_PTR& p, HanhKhach hk) {
	if (p == NULL) {
		p = new nodes_HK;
		p->hk = hk;
		p->left = NULL;
		p->right = NULL;
	}
	else {
		if (strcmp(hk.CMND, p->hk.CMND) < 0) {
			insert_Node_HK(p->left, hk);
		}
		else if (strcmp(hk.CMND, p->hk.CMND) > 0) {
			insert_Node_HK(p->right, hk);
		}
		else {
			return 0;
		}
	}
	return 1;
}

void Preorder(nodesHK_PTR p) { //NLR
	if (p != NULL) {
		cout << "CMND: " << p->hk.CMND << ", Ho ten: "
			<< p->hk.ho << " " << p->hk.ten << ", phai: " << p->hk.phai << endl;
		Preorder(p->left);
		Preorder(p->right);
	}
}

void Inorder(nodesHK_PTR p) { //LNR
	if (p != NULL) {
		Inorder(p->left);
		cout << "CMND: " << p->hk.CMND << ", Ho ten: "
			<< p->hk.ho << " " << p->hk.ten << ", phai: " << p->hk.phai << endl;
		Inorder(p->right);
	}
}

void Posorder(nodesHK_PTR p) { //LRN
	if (p != NULL) {
		Posorder(p->left);
		Posorder(p->right);
		cout << "CMND: " << p->hk.CMND << ", Ho ten: "
			<< p->hk.ho << " " << p->hk.ten << ", phai: " << p->hk.phai << endl;
	}
}

nodesHK_PTR rp;

void remove_case_3(nodesHK_PTR& r) {
	if (r->left != NULL) {
		remove_case_3(r->left);
		//den day r la nut cuc trai cua cay con ben phai co nut goc la rp
	}
	else {
		rp->hk = r->hk; //Chep noi dung cua r sang rp ; de lat nua free(rp)
		rp = r;
		r = rp->right;
	}
}

void removeNodes_HK(nodesHK_PTR& p, char* cmnd) {
	if (p == NULL) {
		cout << "Khong tim thay";
	}
	else {
		if (strcmp(cmnd, p->hk.CMND) < 0) {
			removeNodes_HK(p->left, cmnd);
		}
		else if (strcmp(cmnd, p->hk.CMND) > 0) {
			removeNodes_HK(p->right, cmnd);
		}
		else {
			rp = p;
			if (rp->right == NULL) {
				p = rp->left; // p là nút lá hoac la nut chi co cay con ben trai
			}
			else if (rp->left == NULL) {
				p = rp->right;  // p là nut co cay con ben phai
			}
			else remove_case_3(rp->right);
			delete rp;
		}
	}
}

nodesHK_PTR searchNodes_HK(nodesHK_PTR root, char* cmnd) {
	nodesHK_PTR p = root;
	while (p != NULL && (strcmp(p->hk.CMND, cmnd) != 0)) {
		if (strcmp(cmnd, p->hk.CMND) < 0) {
			p = p->left;
		}
		else { // >0 loai =0
			p = p->right;
		}
	}
	return p;
}

void clearTree_HK(nodesHK_PTR& root) {

}

//////////////////////////////////// doc ghi file////////////////////////////
//may bay
void saveFile_MB(listMB ds, char* filename) {
	FILE* f;
	errno_t err;
	err = fopen_s(&f, filename, "wb");
	if (err != 0) {
		return;
	}
	for (int i = 0; i < ds.n; i++) {
		fwrite(ds.nodes[i], sizeof(MayBay), 1, f);
	}
	fclose(f);
}

void openFile_MB(listMB& ds, char* filename) {
	FILE* f;
	errno_t err;
	err = fopen_s(&f, filename, "rb");
	if (err != 0) {
		return;
	}
	MayBay mb;
	ds.n = 0;
	while (fread(&mb, sizeof(MayBay), 1, f) != 0) {
		insertLast_MB(ds, mb);
	}
	fclose(f);
}

//chuyen bay
void saveFile_CB(nodesCB_PTR first, char* filename) {
	FILE* f;
	errno_t err;
	err = fopen_s(&f, filename, "wb");
	if (err != 0) {
		return;
	}
	for (nodesCB_PTR p = first; p != NULL; p = p->next) {
		fwrite(&p->cb, sizeof(ChuyenBay), 1, f);
		for (int i = 0; i < p->cb.soDay * p->cb.soDong; i++) {
			if (strcmp(p->cb.dsVe.nodes[i].CMND, "trong")) { // chap nhan != 0
				fwrite(&p->cb.dsVe.nodes[i], sizeof(Ve), 1, f);
			}
		}
	}
	fclose(f);
}

void openFile_CB(nodesCB_PTR& first, char* filename) {
	FILE* f;
	errno_t err;
	err = fopen_s(&f, filename, "rb");
	if (err != 0) {
		return;
	}
	ChuyenBay cb;
	while (fread(&cb, sizeof(ChuyenBay), 1, f) != 0) {
		int n = cb.dsVe.n;
		khoiTaoDSVe(cb.dsVe, cb.soDay, cb.soDong);
		cb.dsVe.n = n;
		for (int i = 0; i < cb.dsVe.n; i++) {
			Ve ve;
			fread(&ve, sizeof(Ve), 1, f);
			for (int j = 0; j < cb.soDay * cb.soDong; j++) {
				if (strcmp(cb.dsVe.nodes[j].soVe, ve.soVe) == 0) {
					strcpy_s(cb.dsVe.nodes[j].CMND, ve.CMND);
				}
			}
		}
		insertOrder_CB(first, cb);
	}
	fclose(f);
}

//hanh khach
void Preorder_Write(nodesHK_PTR p, FILE* f) { //NLR
	if (p != NULL) {
		fwrite(&p->hk, sizeof(HanhKhach), 1, f);
		Preorder_Write(p->left, f);
		Preorder_Write(p->right, f);
	}
}

void saveFile_HK(nodesHK_PTR root, char* filename) {
	FILE* f;
	errno_t err;
	err = fopen_s(&f, filename, "wb");
	if (err != 0) {
		return;
	}
	Preorder_Write(root, f);
	fclose(f);
}

void openFile_HK(nodesHK_PTR& root, char* filename) {
	FILE* f;
	errno_t err;
	err = fopen_s(&f, filename, "rb"); // mo duoc file thi bien err=0 ==>ko co loi
	if (err != 0) {
		return;
	}
	HanhKhach hk;
	while (fread(&hk, sizeof(HanhKhach), 1, f) != 0) {
		insert_Node_HK(root, hk);
	}
	fclose(f);
}

//////////////////////////xu ly chuong trinh va giao dien////////////////////

int nhanPhim() {
	int c = _getch();
	if (c == 224) { //nhan phim dac biet
		c = _getch(); //bat gia tri thu 2 cua ma ascii
		c = c + 224;
	}
	else if (c == -32) {
		c = _getch();
	}
	return c;
}

void setFullScreen() {
	system("mode con COLS=700");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}

void normal() {
	SetColor(Black);
	SetBGColor(White);
}

void highlight() {
	SetColor(White);
	SetBGColor(Green);
}

void highlightRed() {
	SetColor(White);
	SetBGColor(Red);
}

int menuDong(char td[][50], int soItem) {
	normal();
	for (int i = 0; i < soItem; i++) {
		gotoxy(cot, dong + i * 2);
		cout << td[i];
	}
	highlight();
	int chon = 0;
	gotoxy(cot, dong + chon);
	cout << td[chon];
	int kyTu;
	do {
		kyTu = nhanPhim();
		switch (kyTu)
		{
		case Up: {
			if (chon >= 0) {
				normal();
				gotoxy(cot, dong + chon * 2);
				cout << td[chon];
				if (chon == 0) {
					chon = soItem - 1;
				}
				else chon--;
				highlight();
				gotoxy(cot, dong + chon * 2);
				cout << td[chon];
			}
			break;
		}
		case Down: {
			if (chon + 1 <= soItem) {
				normal();
				gotoxy(cot, dong + chon * 2);
				cout << td[chon];
				if (chon + 1 == soItem) {
					chon = 0;
				}
				else chon++;
				highlight();
				gotoxy(cot, dong + chon * 2);
				cout << td[chon];
			}
			break;
		}
		case Enter: {
			normal();
			return chon + 1;
		}
		case ESC: {
			return ESC;
		}
		default:
			break;
		}
	} while (true);
}

void box(int x, int y, int ngang, int cao) {
	for (int i = 0; i < ngang; i++) {
		//ngang tren
		gotoxy(x + i, y);
		cout << char(196);
		//ngang duoi
		gotoxy(x + i, y + cao - 1);
		cout << char(196);
	}
	for (int i = 0; i < cao; i++) {
		//dung phai
		gotoxy(x + ngang - 1, y + i);
		cout << char(179);
		//dung trai
		gotoxy(x, y + i);
		cout << char(179);
	}
	gotoxy(x, y);
	cout << char(218); //trai tren
	gotoxy(x + ngang - 1, y);
	cout << char(191); //phai tren
	gotoxy(x, y + cao - 1);
	cout << char(192); // trai duoi
	gotoxy(x + ngang - 1, y + cao - 1);
	cout << char(217); // phai duoi
}

void boxTitle(int trang) {
	SetColor(Red);
	int x = cot;
	int y = dong - 8;
	int ngang = 33;
	int cao = 3;
	if (trang == 6 || trang == 7 || trang == 8
		|| trang == 10 || trang == 11
		|| trang == 14 || trang == 13 || trang == 15) {
		y = dong - 10;
	}
	else if (trang == 19) {
		y = 1;
	}
	box(x, y, ngang, cao);
	//tieu de
	gotoxy(x + 1, y + 1);
	if (trang == 0) { //trang chu
		cout << "   DE 3: QUAN LY CHUYEN BAY";
	}
	else if (trang == 1) {
		SetColor(Black);
		cout << "   1. Quan Ly May Bay";
	}
	else if (trang == 2) {
		SetColor(Black);
		cout << "   2. Quan Ly Chuyen Bay";
	}
	else if (trang == 3) {
		SetColor(Black);
		cout << "   3. Dat / Huy ve Chuyen Bay";
	}
	else if (trang == 4) {
		SetColor(Black);
		cout << "   4. In danh sach / Thong Ke";
	}
	else if (trang == 5) {
		SetColor(Blue);
		cout << "        a. Them may bay";
	}
	else if (trang == 6) {
		SetColor(Blue);
		cout << "        b. Xoa may bay";
	}
	else if (trang == 7) {
		SetColor(Blue);
		cout << "        c. Sua may bay";
	}
	else if (trang == 8) {
		SetColor(Blue);
		cout << "     d. Danh sach may bay";
	}
	else if (trang == 9) {
		SetColor(Blue);
		cout << "      a. Them chuyen bay";
	}
	else if (trang == 10) {
		SetColor(Blue);
		cout << "      b. Xoa chuyen bay";
	}
	else if (trang == 11) {
		SetColor(Blue);
		cout << "      c. Sua chuyen bay";
	}
	else if (trang == 12) {
		SetColor(Blue);
		cout << "      d. Huy chuyen bay";
	}
	else if (trang == 13) {
		SetColor(Blue);
		cout << "   e. Danh sach chuyen bay";
	}
	else if (trang == 14) {
		SetColor(Blue);
		cout << "        a. Chon may bay";
	}
	else if (trang == 15) {
		SetColor(Blue);
		cout << "       a. Chon chuyen bay";
	}
	else if (trang == 16) {
		SetColor(Blue);
		cout << "    Nhap so CMND hanh khach";
	}
	else if (trang == 17) {
		SetColor(Blue);
		cout << "        Them hanh khach";
	}
	else if (trang == 18) {
		SetColor(Blue);
		cout << "     Thong tin hanh khach";
	}
	else if (trang == 19) {
		SetColor(Blue);
		cout << "     Chon so ve chuyen bay";
	}
}

void boxDetail(int trang, int ngang, int cao) {
	int x = 3;
	int y = 10;
	SetColor(Red);
	box(x, y, ngang, cao);
	//chi tiet
	gotoxy(x + 1, y + 1);
	if (trang == 0) {
		SetColor(Blue);
		cout << "         THONG TIN";
		gotoxy(x + 1, y + 2);
		SetColor(Black);
		cout << "  Sinh vien: Nguyen Phu Duy";
		gotoxy(x + 1, y + 3);
		cout << "  Ma SV: N17DCCN029";
		gotoxy(x + 1, y + 4);
		cout << "  Lop: D17CQCN01-N";
		gotoxy(x + 1, y + 5);
		cout << "  GVHD: Luu Nguyen Ky Thu";
	}
	else if (trang == 1) {
		SetColor(Blue);
		cout << "         YEU CAU";
		gotoxy(x + 1, y + 2);
		SetColor(Black);
		cout << "  Ma MB la khoa";
		gotoxy(x + 1, y + 3);
		cout << "  Khong xoa duoc may bay neu";
		gotoxy(x + 1, y + 4);
		cout << "ton tai trong chuyen bay.";
		gotoxy(x + 1, y + 5);
		cout << "  Khong sua duoc so day, ";
		gotoxy(x + 1, y + 6);
		cout << "so dong neu da duoc dat ve.";
	}
	else if (trang == 2) {
		SetColor(Blue);
		cout << "         YEU CAU";
		gotoxy(x + 1, y + 2);
		SetColor(Black);
		cout << "  Ma CB la khoa";
		gotoxy(x + 1, y + 3);
		cout << "  Khong xoa duoc chuyen bay";
		gotoxy(x + 1, y + 4);
		cout << "neu da co ve duoc dat.";
		gotoxy(x + 1, y + 5);
		cout << "  Trang thai duoc tu dong";
		gotoxy(x + 1, y + 6);
		cout << "thay doi tru trang thai huy.";
	}
	else if (trang == 3) {
		SetColor(Blue);
		cout << "         YEU CAU";
		gotoxy(x + 1, y + 2);
		SetColor(Black);
		cout << "  1 cmnd duoc dat toi da 1";
		gotoxy(x + 1, y + 3);
		cout << "ve tren cung 1 chuyen bay.";
		gotoxy(x + 1, y + 4);
		cout << "  2 CB cung MB cach nhau 2h.";
		gotoxy(x + 1, y + 5);
		cout << "  Chi huy ve khi trang thai";
		gotoxy(x + 1, y + 6);
		cout << "chuyen bay chua hoan tat.";
	}
	else if (trang == 4) {
		SetColor(Blue);
		cout << "         YEU CAU";
		gotoxy(x + 1, y + 2);
		SetColor(Black);
		cout << " 1. Dua vao ma chuyen bay.";
		gotoxy(x + 1, y + 3);
		cout << " 2. Ngay dd/mm/yyyy noi den";
		gotoxy(x + 1, y + 4);
		cout << "XXXX va con ve.";
		gotoxy(x + 1, y + 5);
		cout << " 3. Theo ma CB la X.";
		gotoxy(x + 1, y + 6);
		cout << " 4. SL thuc hien giam dan.";
	}
	else if (trang == 5 || trang == 6 || trang == 7) {
		SetColor(Blue);
		cout << "   THONG BAO";
		gotoxy(x + 1, y + 2);
		SetColor(Black);
		if (trang == 5) {
			cout << " Them may bay";
		}
		else if (trang == 6) {
			cout << " Xoa may bay";
		}
		else if (trang == 7) {
			cout << " Sua may bay";
		}
		gotoxy(x + 1, y + 3);
		cout << "  thanh cong!";
		gotoxy(x + 1, y + 4);
		cout << "Ban muon tiep tuc?";
		gotoxy(x + 1, y + 5);
		cout << "(y / n): ";
	}
	else if (trang == 8 || trang == 9 || trang == 10) {
		SetColor(Blue);
		cout << "   THONG BAO";
		gotoxy(x + 1, y + 2);
		SetColor(Black);
		if (trang == 8) {
			cout << "Them chuyen bay";
		}
		else if (trang == 9) {
			cout << "Xoa chuyen bay";
		}
		else if (trang == 10) {
			cout << "Sua chuyen bay";
		}
		gotoxy(x + 1, y + 3);
		cout << "  thanh cong!";
		gotoxy(x + 1, y + 4);
		cout << "Ban muon tiep tuc?";
		gotoxy(x + 1, y + 5);
		cout << "(y / n): ";
	}
	else if (trang == 11) {
		SetColor(Blue);
		cout << "   THONG BAO";
		gotoxy(x + 1, y + 2);
		SetColor(Black);
		cout << "Huy chuyen bay";
		gotoxy(x + 1, y + 3);
		cout << "  thanh cong!";
	}
	else if (trang == 12) {
		SetColor(Blue);
		cout << "   THONG BAO";
		gotoxy(x + 1, y + 2);
		SetColor(Black);
		cout << "Them hanh khach";
		gotoxy(x + 1, y + 3);
		cout << "  thanh cong!";
	}
	else if (trang == 13) {
		SetColor(Blue);
		cout << "THONG BAO";
		gotoxy(x + 1, y + 2);
		SetColor(Black);
		cout << "Dat ve";
		gotoxy(x + 1, y + 3);
		cout << "thanh cong";
		gotoxy(x + 1, y + 4);
		cout << "Tiep tuc";
		gotoxy(x + 1, y + 5);
		cout << "dat?(y/n):";
	}
}

void boxDieuHuong(int trang) {
	int x = 25;
	int y = 24;
	int ngang = 80;
	int cao = 3;
	if (trang == 17) {
		y = 25;
	}
	SetColor(Blue);
	box(x, y, ngang, cao);
	SetColor(Black);
	gotoxy(x + 1, y + 1);
	if (trang == 0) { //trang chu
		cout << "      Su dung phim mui ten len xuong va Enter de chon muc can thao tac";
	}
	else if (trang == 1 || trang == 2 || trang == 3 || trang == 4) {
		cout << "       Su dung phim mui ten len xuong    Enter: chon     ESC: thoat";
	}
	else if (trang == 5) {
		cout << " Ctrl_s: Luu May Bay    Su dung phim mui ten len xuong        ESC: Thoat(huy)";
	}
	else if (trang == 6) {
		cout << " delete: Xoa May Bay    Su dung phim mui ten len xuong        ESC: Thoat(huy)";
	}
	else if (trang == 7) {
		cout << " Tab: Sua May Bay       Su dung phim mui ten len xuong        ESC: Thoat(huy)";
	}
	else if (trang == 8) {
		cout << " Nhap ky tu, Enter de tim kiem   Su dung phim mui ten len xuong   ESC: Thoat";
	}
	else if (trang == 9) {
		cout << " Ctrl_s: Luu Chuyen Bay      Su dung cac phim mui ten         ESC: Thoat(huy)";
	}
	else if (trang == 10) {
		cout << " delete: Xoa Chuyen Bay      Su dung cac phim mui ten         ESC: Thoat(huy)";
	}
	else if (trang == 11) {
		cout << " Tab: Sua Chuyen Bay         Su dung cac phim mui ten         ESC: Thoat(huy)";
	}
	else if (trang == 12) {
		cout << " Insert: Chon May Bay    Su dung phim mui ten len xuong       ESC: Thoat(huy)";
	}
	else if (trang == 13) {
		cout << " Enter: Huy chuyen bay                                        ESC: Thoat(huy)";
	}
	else if (trang == 14) {
		cout << " Insert: Chon Chuyen Bay    Su dung phim mui ten len xuong    ESC: Thoat(huy)";
	}
	else if (trang == 15) {
		cout << " Enter: Tiep tuc                                              ESC: Thoat(huy)";
	}
	else if (trang == 16) {
		cout << " Ctrl_s: Luu Hanh Khach    Su dung phim mui ten len xuong     ESC: Thoat(huy)";
	}
	else if (trang == 17) {
		cout << " Enter:Chon Ve             Su dung cac phim mui ten           ESC: Thoat(huy)";
	}
}

int TrangChu() {
	boxTitle(0);
	boxDetail(0, 30, 7);
	boxDieuHuong(0);
	const int soItem = 5;
	char td[soItem][50] = {
		"      1. Quan ly may bay         ",
		"      2. Quan ly chuyen bay      ",
		"      3. Dat ve / Huy ve         ",
		"      4. Thong ke                ",
		"      5. Thoat chuong trinh      "
	};
	return menuDong(td, soItem);
}

int trangQLMB() {
	boxTitle(1);
	boxDetail(1, 30, 8);
	boxDieuHuong(1);
	const int soItem = 4;
	char td[soItem][50] = {
		"      a. Them may bay            ",
		"      b. Xoa may bay             ",
		"      c. Sua may bay             ",
		"      d. Danh sach may bay       "
	};
	return menuDong(td, soItem);
}

int trangQLCB() {
	boxTitle(2);
	boxDetail(2, 30, 8);
	boxDieuHuong(2);
	const int soItem = 5;
	char td[soItem][50] = {
		"      a. Them chuyen bay         ",
		"      b. Xoa chuyen bay          ",
		"      c. Sua chuyen bay          ",
		"      d. Huy chuyen bay          ",
		"      e. Danh sach chuyen bay    "
	};
	return menuDong(td, soItem);
}

int trangDvHv() {
	boxTitle(3);
	boxDetail(3, 30, 8);
	boxDieuHuong(3);
	const int soItem = 3;
	char td[soItem][50] = {
		"      a. Dat ve                  ",
		"      b. Huy ve                  ",
		"      c. Danh sach khach hang    "
	};
	return menuDong(td, soItem);
}

int trangThongKe() {
	boxTitle(4);
	boxDetail(4, 30, 8);
	boxDieuHuong(4);
	const int soItem = 4;
	char td[soItem][50] = {
		"      a. DS KH thuoc 1 chuyen bay        ",
		"      b. DS cac CB khoi hanh dd/MM/yyyy  ",
		"      c. DS ve con trong                 ",
		"      d. Thong ke luot thuc hien CB      "
	};
	return menuDong(td, soItem);
}

void veKhungNhap(char td[][20], int soItem, int x, int y) {
	for (int i = 0; i < soItem; i++) {
		SetColor(Aqua);
		box(x, y, 70, 3);
		SetColor(Black);
		gotoxy(x + 1, y + 1);
		cout << td[i];
		y = y + 3;
	}
}

void boxBaoLoi(int x, int y, int ngang, int cao) {
	SetColor(Red);
	box(x, y, ngang, cao);
	gotoxy(x + 1, y + 1);
	cout << "       BAO LOI";
}

void xoaBoxTB(int x, int y, int ngang, int cao) {
	for (int j = 0; j < cao; j++) {
		for (int i = 0; i < ngang; i++) {
			gotoxy(x + i, y + j);
			cout << " ";
		}
	}
}

void inLoi(char* err, int h) {
	int a = 97;
	int b = 7;
	SetColor(Black);
	gotoxy(a + 2, b + h);
	cout << err;
}

void inVaXoaTBLoi(int loi) {
	int x_err = 97;
	int y_err = 7;
	int ngang_err = 23;
	int cao_err = 10;
	char err[21];
	boxBaoLoi(x_err, y_err, ngang_err, cao_err);
	if (loi == 1) {
		strcpy_s(err, "Danh sach MB da day");
		inLoi(err, 2);
		strcpy_s(err, "Toi da 300 may bay");
		inLoi(err, 3);
	}
	else if (loi == 2) {
		strcpy_s(err, "Danh sach MB rong");
		inLoi(err, 2);
		strcpy_s(err, "Khong the xoa !");
		inLoi(err, 3);
	}
	else if (loi == 3) {
		strcpy_s(err, "Danh sach MB rong");
		inLoi(err, 2);
		strcpy_s(err, "Khong co MB de sua!");
		inLoi(err, 3);
	}
	else if (loi == 4) {
		strcpy_s(err, "  Xoa may bay");
		inLoi(err, 2);
		strcpy_s(err, "Khong thanh cong!");
		inLoi(err, 3);
	}
	else if (loi == 5) {
		strcpy_s(err, "Ban khong sua gi ca!");
		inLoi(err, 2);
	}
	else if (loi == 6) {
		strcpy_s(err, "  Sua may bay");
		inLoi(err, 2);
		strcpy_s(err, "Khong thanh cong!");
		inLoi(err, 3);
	}
	else if (loi == 7) {
		strcpy_s(err, "Them chuyen bay");
		inLoi(err, 2);
		strcpy_s(err, "Khong thanh cong!");
		inLoi(err, 3);
	}
	else if (loi == 8) {
		strcpy_s(err, "Khong co ket qua!");
		inLoi(err, 2);
	}
	else if (loi == 9) {
		strcpy_s(err, "Vui long nhap tu!");
		inLoi(err, 2);
	}
	else if (10 <= loi && loi <= 13) {
		if (loi == 10) {
			strcpy_s(err, "So hieu may bay");
		}
		else if (loi == 11) {
			strcpy_s(err, "Loai may bay");
		}
		else if (loi == 12) {
			strcpy_s(err, "So day may bay");
		}
		else if (loi == 13) {
			strcpy_s(err, "So dong may bay");
		}
		inLoi(err, 2);
		strcpy_s(err, "khong duoc bo trong!");
		inLoi(err, 3);
	}
	else if (loi == 14) {
		strcpy_s(err, "So hieu MB bi trung");
		inLoi(err, 2);
	}
	else if (loi == 15 || loi == 16) {
		char tmp[3];
		if (loi == 15) {
			strcpy_s(err, "So day toi da la ");
			_itoa_s(MAXDAY, tmp, 10);
		}
		else if (loi == 16) {
			strcpy_s(err, "So dong toi da la ");
			_itoa_s(MAXDONG, tmp, 10);
		}
		strcat_s(err, tmp);
		inLoi(err, 2);
	}
	else if (loi == 17) {
		strcpy_s(err, "So cho it nhat la 20");
		inLoi(err, 2);
		strcpy_s(err, "Socho=soday*sodong");
		inLoi(err, 3);
	}
	else if (loi == 18) {
		strcpy_s(err, "Danh sach MB da day");
		inLoi(err, 2);
		strcpy_s(err, "  Them may bay ");
		inLoi(err, 3);
		strcpy_s(err, "khong thanh cong!");
		inLoi(err, 4);
	}
	else if (19 <= loi && loi <= 22) {
		if (loi == 19) {
			strcpy_s(err, "Ma chuyen bay");
		}
		else if (loi == 20) {
			strcpy_s(err, "Ngay gio khoi hanh");
		}
		else if (loi == 21) {
			strcpy_s(err, "San bay den");
		}
		else if (loi == 22) {
			strcpy_s(err, "So hieu may bay");
		}
		inLoi(err, 2);
		strcpy_s(err, "khong duoc bo trong!");
		inLoi(err, 3);
	}
	else if (loi == 23) {
		strcpy_s(err, "Ma CB bi trung!");
		inLoi(err, 2);
	}
	else if (loi == 24) {
		strcpy_s(err, "Ngay gio khoi hanh");
		inLoi(err, 2);
		strcpy_s(err, "  khong hop le ! ");
		inLoi(err, 3);
	}
	else if (loi == 25) {
		strcpy_s(err, "Ngay gio khoi hanh");
		inLoi(err, 2);
		strcpy_s(err, "khong the o qua khu");
		inLoi(err, 3);
	}
	else if (loi == 26) {
		strcpy_s(err, "May bay khong xoa");
		inLoi(err, 2);
		strcpy_s(err, "duoc vi da ton tai");
		inLoi(err, 3);
		strcpy_s(err, "trong chuyen bay!");
		inLoi(err, 4);
	}
	else if (loi == 27) {
		strcpy_s(err, "May bay chi sua duoc");
		inLoi(err, 2);
		strcpy_s(err, "loai vi da ton tai");
		inLoi(err, 3);
		strcpy_s(err, "mb trong chuyen bay!");
		inLoi(err, 4);
	}
	else if (loi == 28) {
		strcpy_s(err, "May bay nay tai");
		inLoi(err, 2);
		strcpy_s(err, "ngay gio khoi hanh");
		inLoi(err, 3);
		strcpy_s(err, "nay da trung voi");
		inLoi(err, 4);
		strcpy_s(err, "1 chuyen bay khac!");
		inLoi(err, 5);
	}
	else if (loi == 29) {
		gotoxy(x_err + 1, y_err + 1);
		cout << "       THONG BAO";
		strcpy_s(err, "Khoi tao danh sach");
		inLoi(err, 2);
		strcpy_s(err, " ve thanh cong!");
		inLoi(err, 3);
		Sleep(2000);
		return;
	}
	else if (loi == 30) {
		strcpy_s(err, "Danh sach MB rong");
		inLoi(err, 2);
	}
	else if (loi == 31) {
		strcpy_s(err, "Danh sach CB rong");
		inLoi(err, 2);
	}
	else if (loi == 32) {
		strcpy_s(err, "  Xoa chuyen bay");
		inLoi(err, 2);
		strcpy_s(err, "Khong thanh cong!");
		inLoi(err, 3);
	}
	else if (loi == 33) {
		strcpy_s(err, "Chuyen bay khong");
		inLoi(err, 2);
		strcpy_s(err, "xoa duoc vi da co");
		inLoi(err, 3);
		strcpy_s(err, "hanh khach dat ve");
		inLoi(err, 4);
		strcpy_s(err, "CB chi co the huy!");
		inLoi(err, 5);
	}
	else if (loi == 34) {
		strcpy_s(err, "Chuyen bay khong");
		inLoi(err, 2);
		strcpy_s(err, "xoa duoc vi chuyen");
		inLoi(err, 3);
		strcpy_s(err, "bay da hoan tat!");
		inLoi(err, 4);
	}
	else if (loi == 35) {
		strcpy_s(err, "CB chi sua duoc");
		inLoi(err, 2);
		strcpy_s(err, "ngay gio khoi hanh");
		inLoi(err, 3);
		strcpy_s(err, "vi da co hanh");
		inLoi(err, 4);
		strcpy_s(err, "khach dat ve");
		inLoi(err, 5);
	}
	else if (loi == 36) {
		strcpy_s(err, "Chuyen bay khong");
		inLoi(err, 2);
		strcpy_s(err, "sua duoc !");
		inLoi(err, 3);
		strcpy_s(err, "Vi chuyen bay da");
		inLoi(err, 4);
		strcpy_s(err, "hoan tat!");
		inLoi(err, 5);
	}
	else if (loi == 37) {
		strcpy_s(err, "Chuyen bay khong");
		inLoi(err, 2);
		strcpy_s(err, "sua duoc !");
		inLoi(err, 3);
		strcpy_s(err, "Vi chuyen bay da");
		inLoi(err, 4);
		strcpy_s(err, "bi huy!");
		inLoi(err, 5);
	}
	else if (loi == 38) {
		strcpy_s(err, "  Khong ton tai");
		inLoi(err, 2);
		strcpy_s(err, "ma chuyen bay nay!");
		inLoi(err, 3);
	}
	else if (loi == 39) {
		strcpy_s(err, "Chuyen bay nay da");
		inLoi(err, 2);
		strcpy_s(err, "bi huy!");
		inLoi(err, 3);
		strcpy_s(err, "Khong the huy!");
		inLoi(err, 4);
	}
	else if (loi == 40) {
		strcpy_s(err, "Chuyen bay nay da");
		inLoi(err, 2);
		strcpy_s(err, "hoan tat!");
		inLoi(err, 3);
		strcpy_s(err, "Khong the huy!");
		inLoi(err, 4);
	}
	else if (loi == 41) {
		strcpy_s(err, "Hanh khach da dat ve");
		inLoi(err, 2);
		strcpy_s(err, "cho chuyen bay nay");
		inLoi(err, 3);
	}
	else if (loi == 42) {
		strcpy_s(err, "Ho hanh khach");
		inLoi(err, 2);
		strcpy_s(err, "khong duoc bo trong!");
		inLoi(err, 3);
	}
	else if (loi == 43) {
		strcpy_s(err, "Ten hanh khach");
		inLoi(err, 2);
		strcpy_s(err, "khong duoc bo trong!");
		inLoi(err, 3);
	}
	else if (loi == 44) {
		strcpy_s(err, "Ve nay da co");
		inLoi(err, 2);
		strcpy_s(err, "hanh khach dat!");
		inLoi(err, 3);
		strcpy_s(err, "Vui long chon lai!");
		inLoi(err, 4);
	}
	Sleep(2000);
	xoaBoxTB(x_err, y_err, ngang_err, cao_err);
}

bool isAlphabet(int c) {
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
		return true;
	}
	return false;
}

bool isNumber(int c) {
	if (c >= 48 && c <= 57) {
		return true;
	}
	return false;
}

bool xoaKyTu(char* chuoi, int x, int y) {
	if (strlen(chuoi) == 0) {
		return false;
	}
	chuoi[strlen(chuoi) - 1] = '\0';
	gotoxy(x + strlen(chuoi), y);
	cout << " ";
	gotoxy(x + strlen(chuoi), y);
	return true;
}

bool themKyTu(char* chuoi, int size, int c) {
	if (strlen(chuoi) < size - 1) {
		chuoi[strlen(chuoi)] = char(c);
		return true;
	}
	else {
		return false;
	}
}

void chepMangKyTu(char* chuoiNguon, char* chuoiDich, int size) {
	for (int i = 0; i < strlen(chuoiNguon); i++) {
		if (themKyTu(chuoiDich, size, chuoiNguon[i]) == false) {
			break; //thoat duoc for nho do dai chuoidich=chuoinguon
		}
	}
}

int loiThemMB_rong(char* sohieu, char* loai, char* soday, char* sodong) {
	int a = 97;
	int b = 7;
	int viTri = -1;
	char err[21];
	if (strlen(sohieu) == 0 || strlen(loai) == 0 || strlen(soday) == 0 || strlen(sodong) == 0) {
		gotoxy(a + 2, b + 2);
		if (strlen(sohieu) == 0) {
			viTri = 0;
			inVaXoaTBLoi(10);
		}
		else if (strlen(loai) == 0) {
			viTri = 1;
			inVaXoaTBLoi(11);
		}
		else if (strlen(soday) == 0) {
			viTri = 2;
			inVaXoaTBLoi(12);
		}
		else if (strlen(sodong) == 0) {
			viTri = 3;
			inVaXoaTBLoi(13);
		}
	}
	return viTri;
}

void veKhungDS(int x, int y, int ngang, int cao, int tmp[], int soItem) {
	int tmp_x = x;
	box(x, y, ngang, cao);
	for (int i = 0; i < soItem; i++) {
		tmp_x += tmp[i];
		box(tmp_x, y + 2, tmp[i + 1] + 1, cao - 2);
	}
	box(x, y + 4, ngang, cao - 4);
	gotoxy(x, y + 2);
	cout << char(195);
	gotoxy(x, y + 4);
	cout << char(195);
	gotoxy(x + ngang - 1, y + 2);
	cout << char(180);
	gotoxy(x + ngang - 1, y + 4);
	cout << char(180);
	tmp_x = x;
	for (int i = 1; i < soItem; i++) {
		tmp_x += tmp[i];
		gotoxy(tmp_x, y + 2);
		cout << char(194);
		gotoxy(tmp_x, y + 4);
		cout << char(197);
		gotoxy(tmp_x, y + cao - 1);
		cout << char(193);
	}
}

void showTitle_Search(int tmp[], char td[][50], int soItem, int x, int y, int mau) {
	int tmp_x = x + 3;
	SetColor(mau);
	for (int i = 0; i < soItem; i++) {
		tmp_x += tmp[i];
		gotoxy(tmp_x, y + 3);
		cout << td[i];
	}
}

void trangThem_Xoa_SuaMB_theoMode(int x, int y, int mode) {
	if (mode == 0) {//them mb
		boxTitle(5);
		boxDieuHuong(5);
	}
	else if (mode == 1) {//xoa mb
		boxTitle(6);
	}
	else if (mode == 2) {//sua mb
		boxTitle(7);
		boxDieuHuong(5);
	}
	const int soItem = 4;
	char td[soItem][20] = {
		"So hieu may bay  : ",
		"Loai may bay     : ",
		"So day           : ",
		"So dong          : "
	};
	veKhungNhap(td, soItem, x, y);
}

void trangDSMB_TimKiem_Xoa_Sua(int x, int y, int ngang, int cao, int tmp[], int mode) {
	if (mode == 2) { //xoa may bay
		boxTitle(6);
		boxDieuHuong(6);
	}
	else if (mode == 3) { //sua mb
		boxTitle(7);
		boxDieuHuong(7);
	}
	else if (mode == 1) {//chon mb
		boxTitle(14);
		boxDieuHuong(12);
	}
	else { //tim kiem
		boxTitle(8);
		boxDieuHuong(8);
	}
	const int soItem = 4;
	char td[soItem][50] = {
		"So hieu may bay",
		"Loai may bay",
		"So day",
		"So dong"
	};
	veKhungDS(x, y, ngang, cao, tmp, soItem);
	showTitle_Search(tmp, td, soItem, x, y, Red);
	SetColor(Red);
	gotoxy(x + 3, y + 1);
	cout << "Tim kiem theo so hieu MB: ";
	normal();
}

void inThongTin1MB(MayBay* mb, int x, int y, int tmp[]) {
	int tmp_x = x + 3;
	for (int j = 0; j < 4; j++) {
		tmp_x += tmp[j];
		gotoxy(tmp_x, y + 3);
		if (j == 0) {
			cout << mb->soHieu;
		}
		else if (j == 1) {
			cout << mb->loai;
		}
		else if (j == 2) {
			cout << mb->soDay;
		}
		else if (j == 3) {
			cout << mb->soDong;
		}
	}
}

void inDSMBTaiViTri(listMB ds, int tmp[], int x, int y, int viTriIn, int sl) {
	SetColor(Black);
	int dem = 0;
	for (int i = viTriIn; i < ds.n; i++) {
		inThongTin1MB(ds.nodes[i], x, y, tmp);
		y++;
		dem++;
		if (dem == sl) {
			return;
		}
	}
}

void clearKhungTimKiem(int tmp[], int x, int y_min, int sl, int soItem) {
	int tmp_x;
	for (int i = 0; i < sl; i++) {
		tmp_x = x + 3;
		for (int j = 0; j < soItem; j++) {
			tmp_x += tmp[j];
			gotoxy(tmp_x, y_min + 3 + i);
			for (int k = 0; k < tmp[j + 1] - 3; k++) {
				cout << " ";
			}
		}
	}
}

void chiTietMBCanXoaSua(MayBay mb, int x, int y) {
	normal();
	x = x + 20;
	y++;
	gotoxy(x, y);
	cout << mb.soHieu;
	y += 3;
	gotoxy(x, y);
	cout << mb.loai;
	y += 3;
	gotoxy(x, y);
	cout << mb.soDay;
	y += 3;
	gotoxy(x, y);
	cout << mb.soDong;
}

///////////////////////////////xu ly may bay//////////////////////////

int themMayBay(listMB& ds) {
	int x = 25;
	int y = 8;
	trangThem_Xoa_SuaMB_theoMode(x, y, 0);
	x = x + 20;
	y++;
	gotoxy(x, y);
	int y_min = y;
	int viTri = 0; //0<=vitri<=3
	int vt_min = 0;
	int vt_max = 3;
	char sohieu[15] = "";
	char loai[40] = "";
	char soday[3] = "";
	char sodong[3] = "";
	do {
		int c = nhanPhim();
		if (c == Enter) {
			c = Down;
		}
		switch (c)
		{
		case Up: {
			if (viTri > vt_min) {
				viTri--;
				y = y_min + viTri * 3;
				gotoxy(x, y);
			}
			if (viTri == 0) {
				gotoxy(x + strlen(sohieu), y);
			}
			else if (viTri == 1) {
				gotoxy(x + strlen(loai), y);
			}
			else if (viTri == 2) {
				gotoxy(x + strlen(soday), y);
			}
			break;
		}
		case Down: {
			if (viTri < vt_max) {
				viTri++;
				y = y_min + viTri * 3;
				gotoxy(x, y);
			}
			if (viTri == 1) {
				gotoxy(x + strlen(loai), y);
			}
			else if (viTri == 2) {
				gotoxy(x + strlen(soday), y);
			}
			else if (viTri == 3) {
				gotoxy(x + strlen(sodong), y);
			}
			break;
		}
		case Backspace: {
			if (viTri == 0) {
				if (xoaKyTu(sohieu, x, y) == false) {
					break;
				}
			}
			else if (viTri == 1) {
				if (xoaKyTu(loai, x, y) == false) {
					break;
				}
			}
			else if (viTri == 2) {
				if (xoaKyTu(soday, x, y) == false) {
					break;
				}
			}
			else if (viTri == 3) {
				if (xoaKyTu(sodong, x, y) == false) {
					break;
				}
			}
			break;
		}
		case Ctrl_s: {
			int vt = loiThemMB_rong(sohieu, loai, soday, sodong);
			if (vt != -1) { //co loi xay ra
				viTri = vt;
				y = y_min + viTri * 3;
				gotoxy(x, y);
				break;
			}
			if (searchNodes_MB(ds, sohieu) != -1) {
				inVaXoaTBLoi(14);
				viTri = 0;
				y = y_min + viTri * 3;
				gotoxy(x + strlen(sohieu), y);
				break;
			}
			int sday = atoi(soday);
			if (sday > MAXDAY) {
				inVaXoaTBLoi(15);
				viTri = 2;
				y = y_min + viTri * 3;
				gotoxy(x + strlen(soday), y);
				break;
			}
			int sdong = atoi(sodong);
			if (sdong > MAXDONG) {
				inVaXoaTBLoi(16);
				viTri = 3;
				y = y_min + viTri * 3;
				gotoxy(x + strlen(sodong), y);
				break;
			}
			int socho = sday * sdong;
			if (socho < 20) {
				inVaXoaTBLoi(17);
				viTri = 2;
				y = y_min + viTri * 3;
				gotoxy(x + strlen(soday), y);
				break;
			}
			if (loai[strlen(loai) - 1] == Space) {
				loai[strlen(loai) - 1] = '\0';
			}
			MayBay mb;
			strcpy_s(mb.soHieu, sohieu);
			strcpy_s(mb.loai, loai);
			mb.soDay = sday;
			mb.soDong = sdong;
			int check = insertLast_MB(ds, mb);
			if (check == 0) {
				inVaXoaTBLoi(18);
				return 0;
			}
			else {
				int a = 20;
				int b = 7;
				boxDetail(5, a, b);
				int kt = nhanPhim();
				if (kt == char('y') || kt == char('Y')) {
					return 1;
				}
				else return 0;
			}
			break;
		}
		case ESC: {
			return 0;
		}
		default:
			if (viTri == 0) {
				if (isAlphabet(c) == true || isNumber(c) == true) {
					if (themKyTu(sohieu, sizeof(sohieu), c) == true) {
						gotoxy(x, y);
						char tmp = sohieu[strlen(sohieu) - 1];
						if (islower(tmp)) {
							sohieu[strlen(sohieu) - 1] = toupper(tmp);
						}
						cout << sohieu;
					}
					else break;
				}
				else break;
			}
			else if (viTri == 1) {
				if (isAlphabet(c) == true || c == Space) {
					if (strlen(loai) < sizeof(loai) - 1 && c == Space) {
						if (strlen(loai) == 0 || strlen(loai) == sizeof(loai) - 2) {
							break; //space sai da break;
						}
						else if (loai[strlen(loai) - 1] == Space) {
							break;
						}
					} //neu ko break thi space do hop le
					if (strlen(loai) < sizeof(loai) - 1) {
						if (themKyTu(loai, sizeof(loai), c) == true) {
							gotoxy(x, y);
							if (strlen(loai) == 1) {
								if (islower(loai[0])) {
									loai[0] = toupper(loai[0]);
								}
							}
							else {
								char tmp = loai[strlen(loai) - 1];
								if (isupper(tmp)) {
									loai[strlen(loai) - 1] = tolower(tmp);
								}
							}
							cout << loai;
						}
						else break;
					}
					else break;
				}
				else break;
			}
			else if (viTri == 2) {
				if (isNumber(c) == true) {
					if (strlen(soday) < sizeof(soday) - 1 && c == 48) { // so 0 ko nam dau
						if (strlen(soday) == 0) {
							break;
						}
					}
					if (strlen(soday) < sizeof(soday) - 1) {
						if (themKyTu(soday, sizeof(soday), c) == true) {
							gotoxy(x, y);
							cout << soday;
						}
					}
					else break;
				}
				else break;
			}
			else if (viTri == 3) {
				if (isNumber(c) == true) {
					if (strlen(sodong) < sizeof(sodong) - 1 && c == 48) { // so 0 ko nam dau
						if (strlen(sodong) == 0) {
							break;
						}
					}
					if (strlen(sodong) < sizeof(sodong) - 1) {
						if (themKyTu(sodong, sizeof(sodong), c) == true) {
							gotoxy(x, y);
							cout << sodong;
						}
					}
					else break;
				}
				else break;
			}
			break;
		}
	} while (true);
}

int xoaMayBay(listMB& ds, int viTriXoa) {
	int x = 25;
	int y = 8;
	trangThem_Xoa_SuaMB_theoMode(x, y, 1);
	chiTietMBCanXoaSua(*ds.nodes[viTriXoa], x, y);
	gotoxy(x, y + 15);
	cout << "Ban co chac muon xoa may bay nay ?(y/n): ";
	do {
		int c = nhanPhim();
		c = tolower(c);
		switch (c) {
			case char('y') : {
				return deleteNodes_MB(ds, viTriXoa);
			}
			case char('n') : {
				return 0; //xoa ko thanh cong
			}
			default:
				break;
		}
	} while (true);
}

int suaMayBay(listMB& ds, int viTriSua, nodesCB_PTR first) {
	int x = 25;
	int y = 8;
	trangThem_Xoa_SuaMB_theoMode(x, y, 2);
	MayBay mb = *ds.nodes[viTriSua];
	chiTietMBCanXoaSua(mb, x, y);
	x = x + 20;
	y++;
	int y_min = y;
	int viTri = 0; //0<=vitri<=3
	int vt_min = 0;
	int vt_max = 3;
	char sohieu[15] = "";
	char loai[40] = "";
	char soday[3] = "";
	char sodong[3] = "";
	chepMangKyTu(mb.soHieu, sohieu, sizeof(sohieu));
	chepMangKyTu(mb.loai, loai, sizeof(loai));
	char tmp[3] = "";
	_itoa_s(mb.soDay, tmp, 10);
	chepMangKyTu(tmp, soday, sizeof(soday));
	_itoa_s(mb.soDong, tmp, 10);
	chepMangKyTu(tmp, sodong, sizeof(sodong));
	gotoxy(x + strlen(sohieu), y);
	//kiem tra ton tai mb trong cb neu co chi cho sua loai mb
	bool e = false;
	if (searchMBinCB(first, ds.nodes[viTriSua]->soHieu) != NULL) {
		inVaXoaTBLoi(27);
		e = true;
	}
	if (e == true) {
		viTri = 1;
		y = y_min + viTri * 3;
		gotoxy(x + strlen(loai), y);
	}
	do {
		int c = nhanPhim();
		if (c == Enter) {
			c = Down;
		}
		switch (c)
		{
		case Up: {
			if (e == true) break;
			if (viTri > vt_min) {
				viTri--;
				y = y_min + viTri * 3;
				gotoxy(x, y);
			}
			if (viTri == 0) {
				gotoxy(x + strlen(sohieu), y);
			}
			else if (viTri == 1) {
				gotoxy(x + strlen(loai), y);
			}
			else if (viTri == 2) {
				gotoxy(x + strlen(soday), y);
			}
			break;
		}
		case Down: {
			if (e == true) break;
			if (viTri < vt_max) {
				viTri++;
				y = y_min + viTri * 3;
				gotoxy(x, y);
			}
			if (viTri == 1) {
				gotoxy(x + strlen(loai), y);
			}
			else if (viTri == 2) {
				gotoxy(x + strlen(soday), y);
			}
			else if (viTri == 3) {
				gotoxy(x + strlen(sodong), y);
			}
			break;
		}
		case Backspace: {
			if (viTri == 0) {
				if (xoaKyTu(sohieu, x, y) == false) {
					break;
				}
			}
			else if (viTri == 1) {
				if (xoaKyTu(loai, x, y) == false) {
					break;
				}
			}
			else if (viTri == 2) {
				if (xoaKyTu(soday, x, y) == false) {
					break;
				}
			}
			else if (viTri == 3) {
				if (xoaKyTu(sodong, x, y) == false) {
					break;
				}
			}
			break;
		}
		case Ctrl_s: {
			int vt = loiThemMB_rong(sohieu, loai, soday, sodong);
			if (vt != -1) { //co loi xay ra
				viTri = vt;
				y = y_min + viTri * 3;
				gotoxy(x, y);
				break;
			}
			int f = searchNodes_MB(ds, sohieu);
			if (f != -1 && f != viTriSua) {
				inVaXoaTBLoi(14);
				viTri = 0;
				y = y_min + viTri * 3;
				gotoxy(x + strlen(sohieu), y);
				break;
			}
			if (loai[strlen(loai) - 1] == Space) {
				loai[strlen(loai) - 1] = '\0';
			}
			int sday = atoi(soday);
			if (sday > MAXDAY) {
				inVaXoaTBLoi(15);
				viTri = 2;
				y = y_min + viTri * 3;
				gotoxy(x + strlen(soday), y);
				break;
			}
			int sdong = atoi(sodong);
			if (sdong > MAXDONG) {
				inVaXoaTBLoi(16);
				viTri = 3;
				y = y_min + viTri * 3;
				gotoxy(x + strlen(sodong), y);
				break;
			}
			int socho = sday * sdong;
			if (socho < 20) {
				inVaXoaTBLoi(17);
				viTri = 2;
				y = y_min + viTri * 3;
				gotoxy(x + strlen(soday), y);
				break;
			}
			if (strcmp(mb.soHieu, sohieu) == 0 && strcmp(mb.loai, loai) == 0
				&& mb.soDay == sday && mb.soDong == sdong) {
				inVaXoaTBLoi(5);
				return 0;
			}
			strcpy_s(ds.nodes[viTriSua]->soHieu, sohieu);
			strcpy_s(ds.nodes[viTriSua]->loai, loai);
			ds.nodes[viTriSua]->soDay = sday;
			ds.nodes[viTriSua]->soDong = sdong;
			return 1;
			break;
		}
		case ESC: {
			return 0;
		}
		default:
			if (viTri == 0) {
				if (isAlphabet(c) == true || isNumber(c) == true) {
					if (themKyTu(sohieu, sizeof(sohieu), c) == true) {
						gotoxy(x, y);
						char tmp = sohieu[strlen(sohieu) - 1];
						if (islower(tmp)) {
							sohieu[strlen(sohieu) - 1] = toupper(tmp);
						}
						cout << sohieu;
					}
					else break;
				}
				else break;
			}
			else if (viTri == 1) {
				if (isAlphabet(c) == true || c == Space) {
					if (strlen(loai) < sizeof(loai) - 1 && c == Space) {
						if (strlen(loai) == 0 || strlen(loai) == sizeof(loai) - 2) {
							break; //space sai da break;
						}
						else if (loai[strlen(loai) - 1] == Space) {
							break;
						}
					} //neu ko break thi space do hop le
					if (strlen(loai) < sizeof(loai) - 1) {
						if (themKyTu(loai, sizeof(loai), c) == true) {
							gotoxy(x, y);
							if (strlen(loai) == 1) {
								if (islower(loai[0])) {
									loai[0] = toupper(loai[0]);
								}
							}
							else {
								char tmp = loai[strlen(loai) - 1];
								if (isupper(tmp)) {
									loai[strlen(loai) - 1] = tolower(tmp);
								}
							}
							cout << loai;
						}
						else break;
					}
					else break;
				}
				else break;
			}
			else if (viTri == 2) {
				if (isNumber(c) == true) {
					if (strlen(soday) < sizeof(soday) - 1 && c == 48) { // so 0 ko nam dau
						if (strlen(soday) == 0) {
							break;
						}
					}
					if (strlen(soday) < sizeof(soday) - 1) {
						if (themKyTu(soday, sizeof(soday), c) == true) {
							gotoxy(x, y);
							cout << soday;
						}
					}
					else break;
				}
				else break;
			}
			else if (viTri == 3) {
				if (isNumber(c) == true) {
					if (strlen(sodong) < sizeof(sodong) - 1 && c == 48) { // so 0 ko nam dau
						if (strlen(sodong) == 0) {
							break;
						}
					}
					if (strlen(sodong) < sizeof(sodong) - 1) {
						if (themKyTu(sodong, sizeof(sodong), c) == true) {
							gotoxy(x, y);
							cout << sodong;
						}
					}
					else break;
				}
				else break;
			}
			break;
		}
	} while (true);
}

int timKiem_Xoa_SuaMB(listMB& ds, nodesCB_PTR first, int mode) {
	int x = 25;
	int y = 5;
	int ngang = 70;
	int cao = 19;
	int tmp[5] = { 0,19,30,10,10 };//khoang cach de ve box //do rong khung
	int index = 0; // vi tri cua mang ds
	int sl = 13;// 13 dong in du lieu mb, in tai vt=0 cua ds
	int y_min = y + 2;//y in mb duoi vi tri search 2 dong, x khong doi
	int viTri = 0; //0<=vitri<=12 vitri thanh sang
	int vt_min = 0;
	int vt_max = 12;
	char sohieu[15] = "";

	trangDSMB_TimKiem_Xoa_Sua(x, y, ngang, cao, tmp, mode);

	int x_Search = wherex();
	int y_Search = wherey();

	y = y_min; //y in mb ngay vt y_min duoi search 2 dong
	inDSMBTaiViTri(ds, tmp, x, y_min, 0, sl);

	highlight();
	inThongTin1MB(ds.nodes[0], x, y_min, tmp);
	normal();

	gotoxy(x_Search, y_Search);
	do {
		int c = nhanPhim();
		switch (c)
		{
		case Up: {
			if (index - 1 < 0) {
				gotoxy(x_Search + strlen(sohieu), y_Search);
				break;
			}
			if (viTri > vt_min) {
				normal();
				inThongTin1MB(ds.nodes[index], x, y, tmp);
				viTri--;
				y = y_min + viTri;
			}
			else if (viTri == vt_min) {
				clearKhungTimKiem(tmp, x, y_min, sl, 4);
				inDSMBTaiViTri(ds, tmp, x, y_min, index - 1, sl);
			}
			index--;
			highlight();
			inThongTin1MB(ds.nodes[index], x, y, tmp);
			normal();
			gotoxy(x_Search + strlen(sohieu), y_Search);
			break;
		}
		case Down: {
			if (index >= ds.n - 1) {
				gotoxy(x_Search + strlen(sohieu), y_Search);
				break;
			}
			if (viTri < vt_max) {
				normal();
				inThongTin1MB(ds.nodes[index], x, y, tmp);
				viTri++;
				y = y_min + viTri;
			}
			else if (viTri == vt_max) {
				clearKhungTimKiem(tmp, x, y_min, sl, 4);
				inDSMBTaiViTri(ds, tmp, x, y_min, index - sl + 2, sl);
			}
			index++;
			highlight();
			inThongTin1MB(ds.nodes[index], x, y, tmp);
			normal();
			gotoxy(x_Search + strlen(sohieu), y_Search);
			break;
		}
		case Backspace: {
			if (xoaKyTu(sohieu, x_Search, y_Search) == false) {
				break;
			}
			break;
		}
		case Enter: {
			if (strlen(sohieu) == 0) {
				inVaXoaTBLoi(9);
				gotoxy(x_Search, y_Search);
				break;
			}
			int k = searchNodes_MB(ds, sohieu);
			if (k == -1) {
				inVaXoaTBLoi(8);
				gotoxy(x_Search + strlen(sohieu), y_Search);
				break;
			}
			else {
				clearKhungTimKiem(tmp, x, y_min, sl, 4);
				y = y_min;
				index = k;
				viTri = 0;
				inDSMBTaiViTri(ds, tmp, x, y_min, index, sl);
				highlight();
				inThongTin1MB(ds.nodes[index], x, y_min, tmp);
				normal();
				//inDSMbTaiViTri(ds, tmp, x, y_min+1, index+1, sl);
				gotoxy(x_Search + strlen(sohieu), y_Search);
			}
			break;
		}
		case Delete: {
			if (mode == 2) {
				if (searchMBinCB(first, ds.nodes[index]->soHieu) != NULL) {
					inVaXoaTBLoi(26);
					gotoxy(x_Search + strlen(sohieu), y_Search);
					break;
				}
				clrscr();
				int i = xoaMayBay(ds, index);
				if (i == 1) {
					int a = 20;
					int b = 7;
					boxDetail(6, a, b);
					int kt = nhanPhim();
					if (kt == char('y') || kt == char('Y')) {
						return 1;
					}
					else return 0;
				}
				else {
					inVaXoaTBLoi(4);
					return 1;
				}
			}
			break;
		}
		case Tab: {
			if (mode == 3) {
				clrscr();
				int i = suaMayBay(ds, index, first);
				if (i == 1) {
					int a = 20;
					int b = 7;
					boxDetail(7, a, b);
					int kt = nhanPhim();
					if (kt == char('y') || kt == char('Y')) {
						return 1;
					}
					else return 0;
				}
				else {
					inVaXoaTBLoi(6);
					return 1;
				}
			}
			break;
		}
		case Insert: {
			if (mode == 1) {
				return index;//vi tri may bay can chon cho chuyen bay
			}
			break;
		}
		case ESC: {
			if (mode == 1) {
				return -1;// khong chon may bay cho cb
			}
			return 0;
		}
		default:
			if (isAlphabet(c) == true || isNumber(c) == true) {
				if (themKyTu(sohieu, sizeof(sohieu), c) == true) {
					gotoxy(x_Search, y_Search);
					char tmp = sohieu[strlen(sohieu) - 1];
					if (islower(tmp)) {
						sohieu[strlen(sohieu) - 1] = toupper(tmp);
					}
					cout << sohieu;
				}
				else break;
			}
			break;
		}
	} while (true);
}

///////////////////////////////xu ly chuyen bay///////////////////////

int loiThemCB_rong(char* macb, char ngayGioKH[][3],
	char* namKH, char* sanBayDen, char* sohieu) {
	int a = 97;
	int b = 7;
	int viTri = -1;
	char err[21];
	bool tmp = true;
	for (int i = 0; i < 4; i++) {
		if (strlen(ngayGioKH[i]) == 0) {
			tmp = false;
			break;
		}
	}
	if (strlen(macb) == 0 || tmp == false ||
		strlen(namKH) == 0 || strlen(sanBayDen) == 0 || strlen(sohieu) == 0) {
		gotoxy(a + 2, b + 2);
		if (strlen(macb) == 0) {
			viTri = 0;
			inVaXoaTBLoi(19);
		}
		else if (tmp == false) {
			viTri = 1;
			inVaXoaTBLoi(20);
		}
		else if (strlen(sanBayDen) == 0) {
			viTri = 2;
			inVaXoaTBLoi(21);
		}
		else if (strlen(sohieu) == 0) {
			viTri = 3;
			inVaXoaTBLoi(22);
		}
	}
	return viTri;
}

void trangThem_Xoa_SuaCB_theoMode(int x, int y, int mode) {
	if (mode == 0) {//them cb
		boxTitle(9);
		boxDieuHuong(9);
	}
	else if (mode == 1) {//xoa cb
		boxTitle(10);
	}
	else if (mode == 2) {//sua cb
		boxTitle(11);
		boxDieuHuong(9);
	}
	const int soItem = 4;
	char td[soItem][20] = {
		"Ma chuyen bay    : ",
		"Ngay gio KH      : ",
		"San bay den      : ",
		"So hieu may bay  : "
	};
	veKhungNhap(td, soItem, x, y);
	gotoxy(x + 20, y + 4);
	cout << "  :  ,  /  /";
	gotoxy(x + 50, y + 4);
	cout << "(hh:mm,dd/MM/yyyy)";
	if (mode == 1) return;
	gotoxy(x + 50, y + 10);
	normal();
	cout << "Chon MB";
}

nodesCB_PTR checkMB_DateTime_ofCB(nodesCB_PTR first, char* sohieu, datetime dt) {
	for (nodesCB_PTR p = first; p != NULL; p = p->next) {
		if (p->cb.trangThai == 1 || p->cb.trangThai == 2) { //con ve & het ve
			if (strcmp(p->cb.soHieu, sohieu) == 0) {
				if (checkNgayTrungChuyenBay(p->cb.ngayGioKH, dt) == true) {
					return p; //bi trung
				}
			}
		}
	}
	return NULL;
}

void trangDSCB_TimKiem_Xoa_Sua(int x, int y, int ngang, int cao, int tmp[], int mode) {
	if (mode == 2) { //xoa chuyen bay
		boxTitle(10);
		boxDieuHuong(10);
	}
	else if (mode == 3) { //sua cb
		boxTitle(11);
		boxDieuHuong(11);
	}
	else if (mode == 1) {//chon cb
		boxTitle(15);
		boxDieuHuong(14);
	}
	else { //tim kiem
		boxTitle(13);
		boxDieuHuong(8);
	}
	const int soItem = 5;
	char td[soItem][50] = {
		"Ma chuyen bay",
		"Ngay gio khoi hanh",
		"San bay den",
		"So hieu may bay",
		"Trang thai"
	};
	veKhungDS(x, y, ngang, cao, tmp, soItem);
	showTitle_Search(tmp, td, soItem, x, y, Red);
	SetColor(Red);
	gotoxy(x + 3, y + 1);
	cout << "Tim kiem theo ma chuyen bay: ";
	normal();
}

void inThongTin1CB(ChuyenBay cb, int x, int y, int tmp[]) {
	char trangThai[4][12] = { "Huy chuyen","Con ve","Het ve","Hoan tat" };
	int tmp_x = x + 3;
	for (int j = 0; j < 5; j++) {
		tmp_x += tmp[j];
		gotoxy(tmp_x, y + 3);
		if (j == 0) {
			cout << cb.maCB;
		}
		else if (j == 1) {
			if (cb.ngayGioKH.gio < 10) {
				cout << "0";
			}
			cout << cb.ngayGioKH.gio << ":";
			if (cb.ngayGioKH.phut < 10) {
				cout << "0";
			}
			cout << cb.ngayGioKH.phut << ",";
			if (cb.ngayGioKH.ngay < 10) {
				cout << "0";
			}
			cout << cb.ngayGioKH.ngay << "/";
			if (cb.ngayGioKH.thang < 10) {
				cout << "0";
			}
			cout << cb.ngayGioKH.thang << "/" << cb.ngayGioKH.nam;
		}
		else if (j == 2) {
			cout << cb.sanBayDen;
		}
		else if (j == 3) {
			cout << cb.soHieu;
		}
		else if (j == 4) {
			cout << trangThai[cb.trangThai];
		}
	}
}

void inDSCBTaiViTri(nodesCB_PTR hienTai, int tmp[], int x, int y, int sl) {
	SetColor(Black);
	int dem = 0;
	for (nodesCB_PTR p = hienTai; p != NULL; p = p->next) {
		inThongTin1CB(p->cb, x, y, tmp);
		y++;
		dem++;
		if (dem == sl) {
			return;
		}
	}
}

nodesCB_PTR timP_Before(nodesCB_PTR first, nodesCB_PTR p_hienTai) {
	if (strcmp(first->cb.maCB, p_hienTai->cb.maCB) == 0) {
		return NULL;
	}
	for (nodesCB_PTR p = first; p != NULL; p = p->next) {
		if (strcmp(p->next->cb.maCB, p_hienTai->cb.maCB) == 0) {
			return p;
		}
	}
}

nodesCB_PTR timP_After(nodesCB_PTR p_hienTai) {
	return p_hienTai->next;
}

void chiTietCBCanXoaSua(ChuyenBay cb, int x, int y) {
	normal();
	x = x + 20;
	y++;
	gotoxy(x, y);
	cout << cb.maCB;
	y += 3;
	gotoxy(x, y);
	if (cb.ngayGioKH.gio < 10) {
		cout << "0";
	}
	cout << cb.ngayGioKH.gio << ":";
	if (cb.ngayGioKH.phut < 10) {
		cout << "0";
	}
	cout << cb.ngayGioKH.phut << ",";
	if (cb.ngayGioKH.ngay < 10) {
		cout << "0";
	}
	cout << cb.ngayGioKH.ngay << "/";
	if (cb.ngayGioKH.thang < 10) {
		cout << "0";
	}
	cout << cb.ngayGioKH.thang << "/" << cb.ngayGioKH.nam;
	y += 3;
	gotoxy(x, y);
	cout << cb.sanBayDen;
	y += 3;
	gotoxy(x, y);
	cout << cb.soHieu;
}

int themChuyenBay(listMB ds, nodesCB_PTR& first) {
	int x = 25;
	int y = 8;
	trangThem_Xoa_SuaCB_theoMode(x, y, 0);
	x = x + 20;
	y++;
	gotoxy(x, y);
	int x_min = x;
	int y_min = y;
	int viTri = 0; //0<=vitri<=3
	int vt_min = 0;
	int vt_max = 3;
	int index = 0;//vitri nhap ngay gio khoi hanh
	int index_min = 0;
	int index_max = 4;
	//bien tam
	char macb[15] = "";
	char ngayGioKH[4][3] = { "","","","" }; //gio,phut,ngay,thang co 2 chu so
	char nam[5] = "";
	char sanBayDen[50] = "";
	char sohieu[15] = "";
	do {
		int c = nhanPhim();
		if (c == Enter && viTri != 3) {
			c = Down;
		}
		switch (c)
		{
		case Up: {
			if (viTri == 3) {
				gotoxy(x + 30, y);
				normal();
				cout << "Chon MB";
			}
			if (viTri > vt_min) {
				viTri--;
				y = y_min + viTri * 3;
				gotoxy(x, y);
			}
			if (viTri == 0) {
				gotoxy(x + strlen(macb), y);
			}
			else if (viTri == 1) {
				if (index == 4) {
					gotoxy(x_min + index * 3 + strlen(nam), y);
				}
				else gotoxy(x_min + index * 3 + strlen(ngayGioKH[index]), y);
			}
			else if (viTri == 2) {
				gotoxy(x + strlen(sanBayDen), y);
			}
			break;
		}
		case Down: {
			if (viTri < vt_max) {
				viTri++;
				y = y_min + viTri * 3;
				gotoxy(x, y);
			}
			if (viTri == 1) {
				if (index == 4) {
					gotoxy(x_min + index * 3 + strlen(nam), y);
				}
				else gotoxy(x_min + index * 3 + strlen(ngayGioKH[index]), y);
			}
			else if (viTri == 2) {
				gotoxy(x + strlen(sanBayDen), y);
			}
			else if (viTri == 3) {
				gotoxy(x + 30, y);
				highlight();
				cout << "Chon MB";
				normal();
			}
			break;
		}
		case Left: {
			if (viTri == 1) {
				if (index > index_min) {
					index--;
				}
				if (index == 4) {
					gotoxy(x_min + index * 3 + strlen(nam), y);
				}
				else {
					gotoxy(x_min + index * 3 + strlen(ngayGioKH[index]), y);
				}
			}
			break;
		}
		case Right: {
			if (viTri == 1) {
				if (index < index_max) {
					index++;
				}
				if (index == 4) {
					gotoxy(x_min + index * 3 + strlen(nam), y);
				}
				else {
					gotoxy(x_min + index * 3 + strlen(ngayGioKH[index]), y);
				}
			}
			break;
		}
		case Backspace: {
			if (viTri == 0) {
				if (xoaKyTu(macb, x, y) == false) {
					break;
				}
			}
			else if (viTri == 1) {
				if (index == 4) {
					if (xoaKyTu(nam, x_min + index * 3, y) == false) {
						break;
					}
				}
				else {
					if (xoaKyTu(ngayGioKH[index], x_min + index * 3, y) == false) {
						break;
					}
				}
			}
			else if (viTri == 2) {
				if (xoaKyTu(sanBayDen, x, y) == false) {
					break;
				}
			}
			break;
		}
		case Enter: {
			if (viTri == 3) {
				clrscr();
				int i = timKiem_Xoa_SuaMB(ds, first, 1);
				if (i != -1) {
					strcpy_s(sohieu, ds.nodes[i]->soHieu);
				}
				normal();
				clrscr();
				//load lai
				int a = 25;
				int b = 8;
				trangThem_Xoa_SuaCB_theoMode(a, b, 0);
				a += 20;
				b++;
				gotoxy(a, b);
				cout << macb;
				b += 3;
				for (int k = 0; k < 5; k++) {
					gotoxy(a + k * 3, b);
					if (k == 4) cout << nam;
					else cout << ngayGioKH[k];
				}
				b += 3;
				gotoxy(a, b);
				cout << sanBayDen;
				b += 3;
				gotoxy(a, b);
				cout << sohieu;
				gotoxy(x + 30, y);
				highlight();
				cout << "Chon MB";
				normal();
			}
			break;
		}
		case Ctrl_s: {
			int vt = loiThemCB_rong(macb, ngayGioKH, nam, sanBayDen, sohieu);
			if (vt != -1) { //co loi xay ra
				viTri = vt;
				y = y_min + viTri * 3;
				if (viTri == 1) {
					index = 4;//nhay ve cuoi nhap ngay gio khoi hanh
					gotoxy(x_min + index * 3 + strlen(nam), y);
				}
				else if (viTri == 3) {
					gotoxy(x + 30, y);
					highlight();
					cout << "Chon MB";
					normal();
				}
				else gotoxy(x, y);
				break;
			}
			nodesCB_PTR p = searchNodes_CB(first, macb);
			if (p != NULL) { //trung ma chuyen bay
				inVaXoaTBLoi(23);
				viTri = 0;
				y = y_min + viTri * 3;
				gotoxy(x + strlen(macb), y);
				break;
			}
			datetime dt;
			dt.gio = atoi(ngayGioKH[0]);
			dt.phut = atoi(ngayGioKH[1]);
			dt.ngay = atoi(ngayGioKH[2]);
			dt.thang = atoi(ngayGioKH[3]);
			dt.nam = atoi(nam);
			if (checkNgayGioHopLe(dt) == false) {
				inVaXoaTBLoi(24);
				viTri = 1;
				y = y_min + viTri * 3;
				index = 4;//nhay ve cuoi nhap ngay gio khoi hanh
				gotoxy(x_min + index * 3 + strlen(nam), y);
				break;
			}
			// kiem tra thoi gian tuong lai
			if (checkNgayLapChuyenBay(dt) == false) {
				inVaXoaTBLoi(25);
				viTri = 1;
				y = y_min + viTri * 3;
				index = 4;//nhay ve cuoi nhap ngay gio khoi hanh
				gotoxy(x_min + index * 3 + strlen(nam), y);
				break;
			}
			if (checkMB_DateTime_ofCB(first, sohieu, dt) != NULL) {
				inVaXoaTBLoi(28);
				viTri = 3;
				y = y_min + viTri * 3;
				gotoxy(x + 30, y);
				highlight();
				cout << "Chon MB";
				normal();
				break;
			}
			if (sanBayDen[strlen(sanBayDen) - 1] == Space) {
				sanBayDen[strlen(sanBayDen) - 1] = '\0';
			}
			ChuyenBay cb;
			strcpy_s(cb.maCB, macb);
			cb.ngayGioKH = dt;
			strcpy_s(cb.sanBayDen, sanBayDen);
			strcpy_s(cb.soHieu, sohieu);
			int k = searchNodes_MB(ds, sohieu);
			cb.soDay = ds.nodes[k]->soDay;
			cb.soDong = ds.nodes[k]->soDong;
			cb.trangThai = 1;
			int check = khoiTaoDSVe(cb.dsVe, cb.soDay, cb.soDong);
			if (check == 1) {
				inVaXoaTBLoi(29);
			}
			check = insertOrder_CB(first, cb);
			if (check == 0) {
				inVaXoaTBLoi(7);
				return 0;
			}
			else {
				int a = 20;
				int b = 7;
				boxDetail(8, a, b);
				int kt = nhanPhim();
				if (kt == char('y') || kt == char('Y')) {
					return 1;
				}
				else return 0;
			}
			break;
		}
		case ESC: {
			return 0;
		}
		default:
			if (viTri == 0) {
				if (isAlphabet(c) == true || isNumber(c) == true) {
					if (themKyTu(macb, sizeof(macb), c) == true) {
						gotoxy(x, y);
						char tmp = macb[strlen(macb) - 1];
						if (islower(tmp)) {
							macb[strlen(macb) - 1] = toupper(tmp);
						}
						cout << macb;
					}
					else break;
				}
				else break;
			}
			else if (viTri == 1) {
				if (isNumber(c) == true) {
					if (index >= 0 && index <= 3) {
						if (strlen(ngayGioKH[index]) < sizeof(ngayGioKH[index]) - 1) {
							if (themKyTu(ngayGioKH[index], sizeof(ngayGioKH[index]), c) == true) {
								gotoxy(x_min + index * 3, y);
								cout << ngayGioKH[index];
							}
						}
					}
					else if (index == 4) {
						if (strlen(nam) < sizeof(nam) - 1) {
							if (themKyTu(nam, sizeof(nam), c) == true) {
								gotoxy(x_min + index * 3, y);
								cout << nam;
							}
						}
					}
					else break;
				}
				else break;
			}
			else if (viTri == 2) {
				if (isAlphabet(c) == true || c == Space) {
					if (strlen(sanBayDen) < sizeof(sanBayDen) - 1 && c == Space) {
						if (strlen(sanBayDen) == 0 || strlen(sanBayDen) == sizeof(sanBayDen) - 2) {
							break; //space sai da break; space dau va cuoi
						}
						else if (sanBayDen[strlen(sanBayDen) - 1] == Space) {
							break;
						}
					} //neu ko break thi space do hop le
					if (strlen(sanBayDen) < sizeof(sanBayDen) - 1) {
						if (themKyTu(sanBayDen, sizeof(sanBayDen), c) == true) {
							gotoxy(x, y);
							if (strlen(sanBayDen) == 1) {
								if (islower(sanBayDen[0])) {
									sanBayDen[0] = toupper(sanBayDen[0]);
								}
							}
							else if (sanBayDen[strlen(sanBayDen) - 2] == Space) {
								char tmp = sanBayDen[strlen(sanBayDen) - 1];
								if (islower(tmp)) {
									sanBayDen[strlen(sanBayDen) - 1] = toupper(tmp);
								}
							}
							else {
								char tmp = sanBayDen[strlen(sanBayDen) - 1];
								if (isupper(tmp)) {
									sanBayDen[strlen(sanBayDen) - 1] = tolower(tmp);
								}
							}
							cout << sanBayDen;
						}
						else break;
					}
					else break;
				}
				else break;
			}
			break;
		}
	} while (true);
	return 0;
}

int xoaChuyenBay(nodesCB_PTR& first, nodesCB_PTR& p_hienTai) {
	int x = 25;
	int y = 8;
	trangThem_Xoa_SuaCB_theoMode(x, y, 1);
	chiTietCBCanXoaSua(p_hienTai->cb, x, y);
	if (p_hienTai->cb.dsVe.n > 0) { //da co nguoi dat ve thi co the huy
		inVaXoaTBLoi(33);
		gotoxy(x, y + 15);
		cout << "Ban co the muon huy chuyen bay nay ?(y/n): ";
	}
	else {
		gotoxy(x, y + 15);
		cout << "Ban co chac muon xoa chuyen bay nay ?(y/n): ";
	}
	do {
		int c = nhanPhim();
		c = tolower(c);
		switch (c) {
			case char('y') : {
				if (p_hienTai->cb.dsVe.n > 0) {
					p_hienTai->cb.trangThai = 0;
					int a = 20;
					int b = 7;
					boxDetail(11, a, b);
					Sleep(2000);
					return 2;
				}
				//clearList_Ve(p_hienTai->cb.dsVe);
				return deleteNodes_CB(first, p_hienTai->cb.maCB);
			}
			case char('n') : {
				return 0; //xoa ko thanh cong
			}
			default:
				break;
		}
	} while (true);
}

int suaChuyenBay(nodesCB_PTR& first, nodesCB_PTR& p_hienTai, listMB ds) {
	int x = 25;
	int y = 8;
	trangThem_Xoa_SuaCB_theoMode(x, y, 2);
	chiTietCBCanXoaSua(p_hienTai->cb, x, y);
	x = x + 20;
	y++;
	int x_min = x;
	int y_min = y;
	int viTri = 0; //0<=vitri<=3
	int vt_min = 0;
	int vt_max = 3;
	int index = 0;//vitri nhap ngay gio khoi hanh
	int index_min = 0;
	int index_max = 4;
	//bien tam
	char macb[15] = "";
	char ngayGioKH[4][3] = { "","","","" }; //gio,phut,ngay,thang co 2 chu so
	char nam[5] = "";
	char sanBayDen[50] = "";
	char sohieu[15] = "";
	chepMangKyTu(p_hienTai->cb.maCB, macb, sizeof(macb));
	char tmp[3] = "";
	_itoa_s(p_hienTai->cb.ngayGioKH.gio, tmp, 10);
	chepMangKyTu(tmp, ngayGioKH[0], sizeof(ngayGioKH[0]));
	_itoa_s(p_hienTai->cb.ngayGioKH.phut, tmp, 10);
	chepMangKyTu(tmp, ngayGioKH[1], sizeof(ngayGioKH[1]));
	_itoa_s(p_hienTai->cb.ngayGioKH.ngay, tmp, 10);
	chepMangKyTu(tmp, ngayGioKH[2], sizeof(ngayGioKH[2]));
	_itoa_s(p_hienTai->cb.ngayGioKH.thang, tmp, 10);
	chepMangKyTu(tmp, ngayGioKH[3], sizeof(ngayGioKH[3]));
	for (int i = 0; i < 4; i++) {
		if (strlen(ngayGioKH[i]) == 1) { // <10
			ngayGioKH[i][1] = ngayGioKH[i][0];
			ngayGioKH[i][0] = '0';
		}
	}
	char tmp1[5] = "";
	_itoa_s(p_hienTai->cb.ngayGioKH.nam, tmp1, 10);
	chepMangKyTu(tmp1, nam, sizeof(nam));
	chepMangKyTu(p_hienTai->cb.sanBayDen, sanBayDen, sizeof(sanBayDen));
	chepMangKyTu(p_hienTai->cb.soHieu, sohieu, sizeof(sohieu));
	gotoxy(x + strlen(macb), y);
	bool e = false;
	if (p_hienTai->cb.dsVe.n > 0) { //da co nguoi dat ve
		inVaXoaTBLoi(35);
		e = true;
	}
	if (e == true) {
		viTri = 1;
		y = y_min + viTri * 3;
		index = 4;
		gotoxy(x_min + index * 3 + strlen(nam), y);
	}
	do {
		int c = nhanPhim();
		if (c == Enter && viTri != 3) {
			c = Down;
		}
		switch (c)
		{
		case Up: {
			if (e == true) break;
			if (viTri == 3) {
				gotoxy(x + 30, y);
				normal();
				cout << "Chon MB";
			}
			if (viTri > vt_min) {
				viTri--;
				y = y_min + viTri * 3;
				gotoxy(x, y);
			}
			if (viTri == 0) {
				gotoxy(x + strlen(macb), y);
			}
			else if (viTri == 1) {
				if (index == 4) {
					gotoxy(x_min + index * 3 + strlen(nam), y);
				}
				else gotoxy(x_min + index * 3 + strlen(ngayGioKH[index]), y);
			}
			else if (viTri == 2) {
				gotoxy(x + strlen(sanBayDen), y);
			}
			break;
		}
		case Down: {
			if (e == true) break;
			if (viTri < vt_max) {
				viTri++;
				y = y_min + viTri * 3;
				gotoxy(x, y);
			}
			if (viTri == 1) {
				if (index == 4) {
					gotoxy(x_min + index * 3 + strlen(nam), y);
				}
				else gotoxy(x_min + index * 3 + strlen(ngayGioKH[index]), y);
			}
			else if (viTri == 2) {
				gotoxy(x + strlen(sanBayDen), y);
			}
			else if (viTri == 3) {
				gotoxy(x + 30, y);
				highlight();
				cout << "Chon MB";
				normal();
			}
			break;
		}
		case Left: {
			if (viTri == 1) {
				if (index > index_min) {
					index--;
				}
				if (index == 4) {
					gotoxy(x_min + index * 3 + strlen(nam), y);
				}
				else {
					gotoxy(x_min + index * 3 + strlen(ngayGioKH[index]), y);
				}
			}
			break;
		}
		case Right: {
			if (viTri == 1) {
				if (index < index_max) {
					index++;
				}
				if (index == 4) {
					gotoxy(x_min + index * 3 + strlen(nam), y);
				}
				else {
					gotoxy(x_min + index * 3 + strlen(ngayGioKH[index]), y);
				}
			}
			break;
		}
		case Backspace: {
			if (viTri == 0) {
				if (xoaKyTu(macb, x, y) == false) {
					break;
				}
			}
			else if (viTri == 1) {
				if (index == 4) {
					if (xoaKyTu(nam, x_min + index * 3, y) == false) {
						break;
					}
				}
				else {
					if (xoaKyTu(ngayGioKH[index], x_min + index * 3, y) == false) {
						break;
					}
				}
			}
			else if (viTri == 2) {
				if (xoaKyTu(sanBayDen, x, y) == false) {
					break;
				}
			}
			break;
		}
		case Enter: {
			if (viTri == 3) {
				clrscr();
				int i = timKiem_Xoa_SuaMB(ds, first, 1);
				if (i != -1) {
					strcpy_s(sohieu, ds.nodes[i]->soHieu);
				}
				normal();
				clrscr();
				//load lai
				int a = 25;
				int b = 8;
				trangThem_Xoa_SuaCB_theoMode(a, b, 0);
				a += 20;
				b++;
				gotoxy(a, b);
				cout << macb;
				b += 3;
				for (int k = 0; k < 5; k++) {
					gotoxy(a + k * 3, b);
					if (k == 4) cout << nam;
					else cout << ngayGioKH[k];
				}
				b += 3;
				gotoxy(a, b);
				cout << sanBayDen;
				b += 3;
				gotoxy(a, b);
				cout << sohieu;
				gotoxy(x + 30, y);
				highlight();
				cout << "Chon MB";
				normal();
			}
			break;
		}
		case Ctrl_s: {
			int vt = loiThemCB_rong(macb, ngayGioKH, nam, sanBayDen, sohieu);
			if (vt != -1) { //co loi xay ra
				viTri = vt;
				y = y_min + viTri * 3;
				if (viTri == 1) {
					index = 4;//nhay ve cuoi nhap ngay gio khoi hanh
					gotoxy(x_min + index * 3 + strlen(nam), y);
				}
				else if (viTri == 3) {
					gotoxy(x + 30, y);
					highlight();
					cout << "Chon MB";
					normal();
				}
				else gotoxy(x, y);
				break;
			}
			nodesCB_PTR p = searchNodes_CB(first, macb);
			if (p != NULL && p != p_hienTai) { //trung ma chuyen bay //co the so sanh macb
				inVaXoaTBLoi(23);
				viTri = 0;
				y = y_min + viTri * 3;
				gotoxy(x + strlen(macb), y);
				break;
			}
			datetime dt;
			dt.gio = atoi(ngayGioKH[0]);
			dt.phut = atoi(ngayGioKH[1]);
			dt.ngay = atoi(ngayGioKH[2]);
			dt.thang = atoi(ngayGioKH[3]);
			dt.nam = atoi(nam);
			if (checkNgayGioHopLe(dt) == false) {
				inVaXoaTBLoi(24);
				viTri = 1;
				y = y_min + viTri * 3;
				index = 4;//nhay ve cuoi nhap ngay gio khoi hanh
				gotoxy(x_min + index * 3 + strlen(nam), y);
				break;
			}
			// kiem tra thoi gian tuong lai
			if (checkNgayLapChuyenBay(dt) == false) {
				inVaXoaTBLoi(25);
				viTri = 1;
				y = y_min + viTri * 3;
				index = 4;//nhay ve cuoi nhap ngay gio khoi hanh
				gotoxy(x_min + index * 3 + strlen(nam), y);
				break;
			}
			p = checkMB_DateTime_ofCB(first, sohieu, dt);
			if (p != NULL && p != p_hienTai) { //co the so sanh macb
				inVaXoaTBLoi(28);              // so sanh dia chi cua nodescb
				viTri = 3;
				y = y_min + viTri * 3;
				gotoxy(x + 30, y);
				highlight();
				cout << "Chon MB";
				normal();
				break;
			}
			if (sanBayDen[strlen(sanBayDen) - 1] == Space) {
				sanBayDen[strlen(sanBayDen) - 1] = '\0';
			}
			//kiem tra co sua may bay khong
			if (strcmp(macb, p_hienTai->cb.maCB) == 0 &&
				strcmp(sanBayDen, p_hienTai->cb.sanBayDen) == 0 &&
				strcmp(sohieu, p_hienTai->cb.soHieu) == 0 &&
				checkNgayTrungChuyenBay(dt, p_hienTai->cb.ngayGioKH) == true) {
				inVaXoaTBLoi(5);
				return 0;
			}
			if (e == true) { //chi sua ngaygiokh khi chuyen bay da co nguoi dat ve
				p_hienTai->cb.ngayGioKH = dt;
				return 1;
			}
			//xu ly chuyen bay chua co ve duoc dat
			if (strcmp(sohieu, p_hienTai->cb.soHieu) != 0) {
				int k = searchNodes_MB(ds, sohieu);
				if (k == -1) return 0;
				clearList_Ve(p_hienTai->cb.dsVe);//xoa ds ve cu cua mb cu
				p_hienTai->cb.soDay = ds.nodes[k]->soDay;
				p_hienTai->cb.soDong = ds.nodes[k]->soDong;
				khoiTaoDSVe(p_hienTai->cb.dsVe, p_hienTai->cb.soDay,
					p_hienTai->cb.soDong); //khoi tao lai ds ve cua mb moi
			}
			bool sapXep = false;
			if (strcmp(macb, p_hienTai->cb.maCB) != 0) {
				sapXep = true;
			}
			strcpy_s(p_hienTai->cb.maCB, macb);
			strcpy_s(p_hienTai->cb.sanBayDen, sanBayDen);
			strcpy_s(p_hienTai->cb.soHieu, sohieu);
			p_hienTai->cb.ngayGioKH = dt;
			if (sapXep == true) {
				Selection_Sort(first);
			}
			return 1;
			break;
		}
		case ESC: {
			return 0;
		}
		default:
			if (viTri == 0) {
				if (isAlphabet(c) == true || isNumber(c) == true) {
					if (themKyTu(macb, sizeof(macb), c) == true) {
						gotoxy(x, y);
						char tmp = macb[strlen(macb) - 1];
						if (islower(tmp)) {
							macb[strlen(macb) - 1] = toupper(tmp);
						}
						cout << macb;
					}
					else break;
				}
				else break;
			}
			else if (viTri == 1) {
				if (isNumber(c) == true) {
					if (index >= 0 && index <= 3) {
						if (strlen(ngayGioKH[index]) < sizeof(ngayGioKH[index]) - 1) {
							if (themKyTu(ngayGioKH[index], sizeof(ngayGioKH[index]), c) == true) {
								gotoxy(x_min + index * 3, y);
								cout << ngayGioKH[index];
							}
						}
					}
					else if (index == 4) {
						if (strlen(nam) < sizeof(nam) - 1) {
							if (themKyTu(nam, sizeof(nam), c) == true) {
								gotoxy(x_min + index * 3, y);
								cout << nam;
							}
						}
					}
					else break;
				}
				else break;
			}
			else if (viTri == 2) {
				if (isAlphabet(c) == true || c == Space) {
					if (strlen(sanBayDen) < sizeof(sanBayDen) - 1 && c == Space) {
						if (strlen(sanBayDen) == 0 || strlen(sanBayDen) == sizeof(sanBayDen) - 2) {
							break; //space sai da break; space dau va cuoi
						}
						else if (sanBayDen[strlen(sanBayDen) - 1] == Space) {
							break;
						}
					} //neu ko break thi space do hop le
					if (strlen(sanBayDen) < sizeof(sanBayDen) - 1) {
						if (themKyTu(sanBayDen, sizeof(sanBayDen), c) == true) {
							gotoxy(x, y);
							if (strlen(sanBayDen) == 1) {
								if (islower(sanBayDen[0])) {
									sanBayDen[0] = toupper(sanBayDen[0]);
								}
							}
							else if (sanBayDen[strlen(sanBayDen) - 2] == Space) {
								char tmp = sanBayDen[strlen(sanBayDen) - 1];
								if (islower(tmp)) {
									sanBayDen[strlen(sanBayDen) - 1] = toupper(tmp);
								}
							}
							else {
								char tmp = sanBayDen[strlen(sanBayDen) - 1];
								if (isupper(tmp)) {
									sanBayDen[strlen(sanBayDen) - 1] = tolower(tmp);
								}
							}
							cout << sanBayDen;
						}
						else break;
					}
					else break;
				}
				else break;
			}
			break;
		}
	} while (true);
	return 0;
}

int timKiem_Xoa_SuaCB(nodesCB_PTR& first, listMB ds, int mode) {
	int x = 0;
	int y = 5;
	int ngang = 95;
	int cao = 19;
	int tmp[6] = { 0,19,21,20,19,15 };//khoang cach de ve box //do rong khung
	int sl = 13;// 13 dong in du lieu mb, in tai vt=0 cua ds
	int y_min = y + 2;//y in mb duoi vi tri search 2 dong, x khong doi
	int viTri = 0; //0<=vitri<=12 vitri thanh sang
	int vt_min = 0;
	int vt_max = 12;
	char macb[15] = "";

	trangDSCB_TimKiem_Xoa_Sua(x, y, ngang, cao, tmp, mode);

	int x_Search = wherex();
	int y_Search = wherey();

	y = y_min; //y in mb ngay vt y_min duoi search 2 dong
	nodesCB_PTR p_hienTai = first; //node luu giu vi tri hien tai trong ds
	inDSCBTaiViTri(p_hienTai, tmp, x, y_min, sl);

	highlight();
	inThongTin1CB(p_hienTai->cb, x, y_min, tmp);
	normal();

	gotoxy(x_Search, y_Search);
	do {
		int c = nhanPhim();
		switch (c)
		{
		case Up: {
			if (timP_Before(first, p_hienTai) == NULL) {
				break;
			}
			if (viTri > vt_min) {
				normal();
				inThongTin1CB(p_hienTai->cb, x, y, tmp);
				viTri--;
				y = y_min + viTri;
			}
			else if (viTri == vt_min) {
				clearKhungTimKiem(tmp, x, y_min, sl, 5);
				inDSCBTaiViTri(timP_Before(first, p_hienTai),
					tmp, x, y_min, sl);
			}
			p_hienTai = timP_Before(first, p_hienTai);
			highlight();
			inThongTin1CB(p_hienTai->cb, x, y, tmp);
			normal();
			gotoxy(x_Search + strlen(macb), y_Search);
			break;
		}
		case Down: {
			if (timP_After(p_hienTai) == NULL) {
				break;
			}
			if (viTri < vt_max) {
				normal();
				inThongTin1CB(p_hienTai->cb, x, y, tmp);
				viTri++;
				y = y_min + viTri;
			}
			else if (viTri == vt_max) {
				clearKhungTimKiem(tmp, x, y_min, sl, 5);
				nodesCB_PTR f = p_hienTai;
				for (int i = 0; i < sl - 2; i++) {
					f = timP_Before(first, f);
				}
				inDSCBTaiViTri(f, tmp, x, y_min, sl);
			}
			p_hienTai = timP_After(p_hienTai);
			highlight();
			inThongTin1CB(p_hienTai->cb, x, y, tmp);
			normal();
			gotoxy(x_Search + strlen(macb), y_Search);
			break;
		}
		case Backspace: {
			if (xoaKyTu(macb, x_Search, y_Search) == false) {
				break;
			}
			break;
		}
		case Enter: {
			if (strlen(macb) == 0) {
				inVaXoaTBLoi(9);
				gotoxy(x_Search, y_Search);
				break;
			}
			nodesCB_PTR k = searchNodes_CB(first, macb);
			if (k == NULL) {
				inVaXoaTBLoi(8);
				gotoxy(x_Search + strlen(macb), y_Search);
				break;
			}
			else {
				clearKhungTimKiem(tmp, x, y_min, sl, 5);
				y = y_min;
				p_hienTai = k;
				viTri = 0;
				inDSCBTaiViTri(p_hienTai, tmp, x, y_min, sl);
				highlight();
				inThongTin1CB(p_hienTai->cb, x, y, tmp);
				normal();
				gotoxy(x_Search + strlen(macb), y_Search);
			}
			break;
		}
		case Delete: {
			if (mode == 2) {
				if (p_hienTai->cb.trangThai == 3) { //chuyen bay da hoan tat
					inVaXoaTBLoi(34);
					gotoxy(x_Search + strlen(macb), y_Search);
					break;
				}
				clrscr();
				int i = xoaChuyenBay(first, p_hienTai);
				if (i == 1) {
					int a = 20;
					int b = 7;
					boxDetail(9, a, b);
					int kt = nhanPhim();
					if (kt == char('y') || kt == char('Y')) {
						return 1;
					}
					else return 0;
				}
				else if (i == 2) {
					return 1;
				}
				else {
					inVaXoaTBLoi(32);
					return 1;
				}
			}
			break;
		}
		case Tab: {
			if (mode == 3) {
				if (p_hienTai->cb.trangThai == 3) { //chuyen bay da hoan tat
					inVaXoaTBLoi(36);
					gotoxy(x_Search + strlen(macb), y_Search);
					break;
				}
				else if (p_hienTai->cb.trangThai == 0) { //chuyen bay da bi huy
					inVaXoaTBLoi(37);
					gotoxy(x_Search + strlen(macb), y_Search);
					break;
				}
				clrscr();
				int i = suaChuyenBay(first, p_hienTai, ds);
				if (i == 1) {
					int a = 20;
					int b = 7;
					boxDetail(7, a, b);
					int kt = nhanPhim();
					if (kt == char('y') || kt == char('Y')) {
						return 1;
					}
					else return 0;
				}
				else {
					inVaXoaTBLoi(6);
					return 1;
				}
			}
			break;
		}
		case ESC: {
			if (mode == 1) {
				return -1;// khong chon may bay cho cb
			}
			return 0;
		}
		default:
			if (isAlphabet(c) == true || isNumber(c) == true) {
				if (themKyTu(macb, sizeof(macb), c) == true) {
					gotoxy(x_Search, y_Search);
					char tmp = macb[strlen(macb) - 1];
					if (islower(tmp)) {
						macb[strlen(macb) - 1] = toupper(tmp);
					}
					cout << macb;
				}
				else break;
			}
			break;
		}
	} while (true);
}

int huyChuyenBay(nodesCB_PTR& first) {
	int x = 25;
	int y = 8;
	boxTitle(12);
	boxDieuHuong(13);
	gotoxy(x + 1, y + 6);
	cout << "Nhap ma chuyen bay can huy: ";
	SetColor(Aqua);
	box(x, y + 5, 70, 3);
	SetColor(Black);
	x = x + strlen("Nhap ma chuyen bay can huy: ") + 1;
	y += 6;
	char macb[15] = "";
	gotoxy(x, y);
	do {
		int c = nhanPhim();
		switch (c)
		{
		case Backspace: {
			if (xoaKyTu(macb, x, y) == false) {
				break;
			}
			break;
		}
		case Enter: {
			if (strlen(macb) == 0) {
				inVaXoaTBLoi(9);
				gotoxy(x, y);
				break;
			}
			nodesCB_PTR p = searchNodes_CB(first, macb);
			if (p == NULL) {
				inVaXoaTBLoi(38);
				gotoxy(x + strlen(macb), y);
				break;
			}
			else if (p->cb.trangThai == 0) {
				inVaXoaTBLoi(39);
				gotoxy(x + strlen(macb), y);
				break;
			}
			else if (p->cb.trangThai == 3) {
				inVaXoaTBLoi(40);
				gotoxy(x + strlen(macb), y);
				break;
			}
			gotoxy(26, y + 8);
			cout << "Ban co chac muon huy chuyen bay nay ?(y/n): ";
			char check = nhanPhim();
			if (check == 'y' || check == 'Y') {
				p->cb.trangThai = 0;
				int a = 20;
				int b = 7;
				boxDetail(11, a, b);
				Sleep(2000);
				return 1;
			}
			else {
				gotoxy(26, y + 8);
				cout << "                                             ";
				gotoxy(x + strlen(macb), y);
				break;
			}
			break;
		}
		case ESC: {
			return 0;
		}
		default:
			if (isAlphabet(c) == true || isNumber(c) == true) {
				if (themKyTu(macb, sizeof(macb), c) == true) {
					gotoxy(x, y);
					char tmp = macb[strlen(macb) - 1];
					if (islower(tmp)) {
						macb[strlen(macb) - 1] = toupper(tmp);
					}
					cout << macb;
				}
				else break;
			}
			else break;
			break;
		}
	} while (true);
}

///////////////////////////////xu ly hanh khach dat-huy ve/////////////////

void inDSCB_ConVeTaiViTri(nodesCB_PTR hienTai, int tmp[], int x, int y, int sl) {
	SetColor(Black);
	int dem = 0;
	for (nodesCB_PTR p = hienTai; p != NULL; p = p->next) {
		if (p->cb.trangThai != 1) continue;
		inThongTin1CB(p->cb, x, y, tmp);
		y++;
		dem++;
		if (dem == sl) {
			return;
		}
	}
}

nodesCB_PTR chonChuyenBayConVe(nodesCB_PTR first) {
	int x = 0;
	int y = 5;
	int ngang = 95;
	int cao = 19;
	int tmp[6] = { 0,19,21,20,19,15 };//khoang cach de ve box //do rong khung
	int sl = 13;// 13 dong in du lieu mb, in tai vt=0 cua ds
	int y_min = y + 2;//y in mb duoi vi tri search 2 dong, x khong doi
	int viTri = 0; //0<=vitri<=12 vitri thanh sang
	int vt_min = 0;
	int vt_max = 12;
	char macb[15] = "";

	trangDSCB_TimKiem_Xoa_Sua(x, y, ngang, cao, tmp, 1);

	int x_Search = wherex();
	int y_Search = wherey();

	y = y_min; //y in mb ngay vt y_min duoi search 2 dong
	nodesCB_PTR p_DauConVe = NULL;  // p dau tien con ve
	for (nodesCB_PTR p = first; p != NULL; p = p->next) {
		if (p->cb.trangThai == 1) { //con ve
			p_DauConVe = p;
			break;
		}
	}
	if (p_DauConVe == NULL) return NULL;
	nodesCB_PTR p_hienTai = p_DauConVe;
	inDSCB_ConVeTaiViTri(p_hienTai, tmp, x, y_min, sl);

	highlight();
	inThongTin1CB(p_hienTai->cb, x, y_min, tmp);
	normal();

	gotoxy(x_Search, y_Search);
	do {
		int c = nhanPhim();
		switch (c)
		{
		case Up: {
			if (p_DauConVe == p_hienTai) {
				break;
			}
			if (viTri > vt_min) {
				normal();
				inThongTin1CB(p_hienTai->cb, x, y, tmp);
				viTri--;
				y = y_min + viTri;
			}
			else if (viTri == vt_min) {
				clearKhungTimKiem(tmp, x, y_min, sl, 5);
				nodesCB_PTR p_tmp = p_hienTai;
				while (true) {
					p_tmp = timP_Before(p_DauConVe, p_tmp);
					if (p_tmp->cb.trangThai == 1) {
						break;
					}
				}
				inDSCB_ConVeTaiViTri(p_tmp, tmp, x, y_min, sl);
			}
			nodesCB_PTR p_tmp = p_hienTai;
			while (true) {
				p_tmp = timP_Before(p_DauConVe, p_tmp);
				if (p_tmp->cb.trangThai == 1) {
					break;
				}
			}
			p_hienTai = p_tmp;
			highlight();
			inThongTin1CB(p_hienTai->cb, x, y, tmp);
			normal();
			gotoxy(x_Search + strlen(macb), y_Search);
			break;
		}
		case Down: {
			nodesCB_PTR p_tmp = p_hienTai;
			while (true) {
				p_tmp = timP_After(p_tmp);
				if (p_tmp == NULL) break;
				else if (p_tmp->cb.trangThai != 1) continue;
				else break;
			}
			if (p_tmp == NULL) break;
			if (viTri < vt_max) {
				normal();
				inThongTin1CB(p_hienTai->cb, x, y, tmp);
				viTri++;
				y = y_min + viTri;
			}
			else if (viTri == vt_max) {
				clearKhungTimKiem(tmp, x, y_min, sl, 5);
				nodesCB_PTR f = p_hienTai;
				for (int i = 0; i < sl - 2; i++) {
					f = timP_Before(p_DauConVe, f);
					if (f->cb.trangThai != 1) {
						i--;
					}
				}
				inDSCB_ConVeTaiViTri(f, tmp, x, y_min, sl);
			}
			p_hienTai = p_tmp;
			highlight();
			inThongTin1CB(p_hienTai->cb, x, y, tmp);
			normal();
			gotoxy(x_Search + strlen(macb), y_Search);
			break;
		}
		case Backspace: {
			if (xoaKyTu(macb, x_Search, y_Search) == false) {
				break;
			}
			break;
		}
		case Enter: {
			if (strlen(macb) == 0) {
				inVaXoaTBLoi(9);
				gotoxy(x_Search, y_Search);
				break;
			}
			nodesCB_PTR k = searchNodes_CB(first, macb);
			if (k == NULL || k->cb.trangThai != 1) {
				inVaXoaTBLoi(8);
				gotoxy(x_Search + strlen(macb), y_Search);
				break;
			}
			else {
				clearKhungTimKiem(tmp, x, y_min, sl, 5);
				y = y_min;
				p_hienTai = k;
				viTri = 0;
				inDSCB_ConVeTaiViTri(p_hienTai, tmp, x, y_min, sl);
				highlight();
				inThongTin1CB(p_hienTai->cb, x, y, tmp);
				normal();
				gotoxy(x_Search + strlen(macb), y_Search);
			}
			break;
		}
		case Insert: {
			return p_hienTai;
			break;
		}
		case ESC: {
			return NULL;
		}
		default:
			if (isAlphabet(c) == true || isNumber(c) == true) {
				if (themKyTu(macb, sizeof(macb), c) == true) {
					gotoxy(x_Search, y_Search);
					char tmp = macb[strlen(macb) - 1];
					if (islower(tmp)) {
						macb[strlen(macb) - 1] = toupper(tmp);
					}
					cout << macb;
				}
				else break;
			}
			break;
		}
	} while (true);
}

bool checkHK_thuocCB(nodesCB_PTR p_hienTai, char* cmnd) {
	for (int i = 0; i < p_hienTai->cb.soDay * p_hienTai->cb.soDong; i++) {
		if (strcmp(p_hienTai->cb.dsVe.nodes[i].CMND, cmnd) == 0) {
			return true; //trung
		}
	}
	return false;
}

int checkChuyenBayHopThoiGianDeDatVe(nodesCB_PTR first, nodesCB_PTR p_hienTai, char* cmnd) {
	for (nodesCB_PTR p = first; p != NULL; p = p->next) {
		if (p == p_hienTai) continue;
		if (p->cb.trangThai == 1 || p->cb.trangThai == 2) {
			if (checkHK_thuocCB(p, cmnd) == true) {

			}
		}
	}
	return 1; //hop le
}

void trangThemHK(int x, int y) {
	boxTitle(17);
	boxDieuHuong(16);
	const int soItem = 4;
	char td[soItem][20] = {
		"So CMND          : ",
		"Ho hanh khach    : ",
		"Ten hanh khach   : ",
		"Phai             : "
	};
	veKhungNhap(td, soItem, x, y);
}

int loiThemHK_rong(char* ho, char* ten) {
	int a = 97;
	int b = 7;
	int viTri = -1;
	char err[21];
	if (strlen(ho) == 0 || strlen(ten) == 0) {
		gotoxy(a + 2, b + 2);
		if (strlen(ho) == 0) {
			viTri = 1;
			inVaXoaTBLoi(42);
		}
		else if (strlen(ten) == 0) {
			viTri = 2;
			inVaXoaTBLoi(43);
		}
	}
	return viTri;
}

nodesHK_PTR themHanhKhach(nodesHK_PTR& root, char* cmnd) {
	int x = 25;
	int y = 8;
	trangThemHK(x, y);
	x = x + 20;
	y++;
	gotoxy(x, y);
	cout << cmnd;
	int y_min = y;
	int viTri = 1; //1<=vitri<=3
	int vt_min = 1;
	int vt_max = 3;
	char ten[10] = "";
	char ho[50] = "";
	char phai[2][4] = { "Nam","Nu" };
	int gt = 0;// 0 nam 1 nu
	gotoxy(x, y_min + vt_max * 3);
	highlight();
	cout << phai[gt];
	normal();
	gotoxy(x + 1 * 6, y_min + vt_max * 3);
	cout << phai[1];
	gotoxy(x, y_min + vt_min * 3);
	y = y_min + 1 * 3;
	do {
		int c = nhanPhim();
		if (c == Enter) {
			c = Down;
		}
		switch (c)
		{
		case Up: {
			if (viTri > vt_min) {
				viTri--;
				y = y_min + viTri * 3;
				gotoxy(x, y);
			}
			if (viTri == 1) {
				gotoxy(x + strlen(ho), y);
			}
			else if (viTri == 2) {
				gotoxy(x + strlen(ten), y);
			}
			break;
		}
		case Down: {
			if (viTri < vt_max) {
				viTri++;
				y = y_min + viTri * 3;
				gotoxy(x, y);
			}
			if (viTri == 2) {
				gotoxy(x + strlen(ten), y);
			}
			else if (viTri == 3) { //gioi tinh
				gotoxy(x + gt * 6 + strlen(phai[gt]), y_min + vt_max * 3);
			}
			break;
		}
		case Left: {
			if (gt == 0 || viTri != 3) {
				break;
			}
			gotoxy(x + gt * 6, y_min + vt_max * 3);
			normal();
			cout << phai[gt];

			gt = 0;
			gotoxy(x + gt * 6, y_min + vt_max * 3);
			highlight();
			cout << phai[gt];
			normal();
			break;
		}
		case Right: {
			if (gt == 1 || viTri != 3) {
				break;
			}
			gotoxy(x + gt * 6, y_min + vt_max * 3);
			normal();
			cout << phai[gt];

			gt = 1;
			gotoxy(x + gt * 6, y_min + vt_max * 3);
			highlight();
			cout << phai[gt];
			normal();
			break;
		}
		case Backspace: {
			if (viTri == 1) {
				if (xoaKyTu(ho, x, y) == false) {
					break;
				}
			}
			else if (viTri == 2) {
				if (xoaKyTu(ten, x, y) == false) {
					break;
				}
			}
			break;
		}
		case Ctrl_s: {
			int vt = loiThemHK_rong(ho, ten);
			if (vt != -1) { //co loi xay ra
				viTri = vt;
				y = y_min + viTri * 3;
				gotoxy(x, y);
				break;
			}
			if (ho[strlen(ho) - 1] == Space) {
				ho[strlen(ho) - 1] = '\0';
			}
			HanhKhach hk;
			strcpy_s(hk.CMND, cmnd);
			strcpy_s(hk.ho, ho);
			strcpy_s(hk.ten, ten);
			strcpy_s(hk.phai, phai[gt]);
			if (insert_Node_HK(root, hk) == 0) {
				cout << "them loi";
				break;
			}
			else {
				int a = 20;
				int b = 7;
				boxDetail(12, a, b);
				Sleep(2000);
			}
			return searchNodes_HK(root, hk.CMND);
			break;
		}
		case ESC: {
			return NULL;
		}
		default:
			if (viTri == 1) {
				if (isAlphabet(c) == true || c == Space) {
					if (strlen(ho) < sizeof(ho) - 1 && c == Space) {
						if (strlen(ho) == 0 || strlen(ho) == sizeof(ho) - 2) {
							break; //space sai da break; space dau va cuoi
						}
						else if (ho[strlen(ho) - 1] == Space) {
							break;
						}
					} //neu ko break thi space do hop le
					if (strlen(ho) < sizeof(ho) - 1) {
						if (themKyTu(ho, sizeof(ho), c) == true) {
							gotoxy(x, y);
							if (strlen(ho) == 1) {
								if (islower(ho[0])) {
									ho[0] = toupper(ho[0]);
								}
							}
							else if (ho[strlen(ho) - 2] == Space) {
								char tmp = ho[strlen(ho) - 1];
								if (islower(tmp)) {
									ho[strlen(ho) - 1] = toupper(tmp);
								}
							}
							else {
								char tmp = ho[strlen(ho) - 1];
								if (isupper(tmp)) {
									ho[strlen(ho) - 1] = tolower(tmp);
								}
							}
							cout << ho;
						}
						else break;
					}
					else break;
				}
				else break;
			}
			else if (viTri == 2) {
				if (isAlphabet(c) == true) {
					if (themKyTu(ten, sizeof(ten), c) == true) {
						gotoxy(x, y);
						char tmp = ten[strlen(ten) - 1];
						if (isupper(tmp)) {
							ten[strlen(ten) - 1] = tolower(tmp);
						}
						if (islower(ten[0])) {
							ten[0] = toupper(ten[0]);
						}
						cout << ten;
					}
					else break;
				}
				else break;
			}
			break;
		}
	} while (true);
}

void trangShowHK(int x, int y) {
	boxTitle(18);
	boxDieuHuong(15);
	const int soItem = 4;
	char td[soItem][20] = {
		"So CMND          : ",
		"Ho hanh khach    : ",
		"Ten hanh khach   : ",
		"Phai             : "
	};
	veKhungNhap(td, soItem, x, y);
}

void showHanhKhach(nodesHK_PTR p) {
	char phai[2][4] = { "Nam","Nu" };
	int x = 25;
	int y = 8;
	trangShowHK(x, y);
	normal();
	x = x + 20;
	y++;
	gotoxy(x, y);
	cout << p->hk.CMND;
	y += 3;
	gotoxy(x, y);
	cout << p->hk.ho;
	y += 3;
	gotoxy(x, y);
	cout << p->hk.ten;
	y += 3;
	gotoxy(x, y);
	if (strcmp(p->hk.phai, phai[0]) == 0) {
		highlight();
		cout << p->hk.phai;
		normal();
		gotoxy(x + 1 * 6, y);
		cout << phai[1];
	}
	else {
		cout << phai[0];
		gotoxy(x + 1 * 6, y);
		highlight();
		cout << p->hk.phai;
		normal();
	}
}

int chonSoVe(nodesCB_PTR p_hienTai, nodesHK_PTR hk) {
	boxTitle(19);
	boxDieuHuong(17);
	int x_min = 16;
	int y_min = 5;
	int viTri_day = 0; // < p_hienTai->cb.soDay
	int viTri_dong = 0; // < p_hienTai->cb.soDong
	int vtDong_min = 0;
	int vtDong_max = p_hienTai->cb.soDong - 1;
	int vtDay_min = 0;
	int vtDay_max = p_hienTai->cb.soDay - 1;
	// in danh sach ve
	bool veDauTrong = false;
	for (int vtday = 0; vtday < p_hienTai->cb.soDay; vtday++) {
		for (int vtdong = 0; vtdong < p_hienTai->cb.soDong; vtdong++) {
			gotoxy(x_min + vtdong * 4, y_min + vtday * 2);
			int i = vtdong + vtday * p_hienTai->cb.soDong;
			if (strcmp(p_hienTai->cb.dsVe.nodes[i].CMND, "trong") != 0) {
				highlightRed();
				cout << p_hienTai->cb.dsVe.nodes[i].soVe;
				normal();
			}
			else cout << p_hienTai->cb.dsVe.nodes[i].soVe;
			if (veDauTrong == false && strcmp(p_hienTai->cb.dsVe.nodes[i].CMND, "trong") == 0) {
				viTri_day = vtday;
				viTri_dong = vtdong;
				veDauTrong = true;
			}
		}
	}
	gotoxy(x_min + viTri_dong * 4, y_min + viTri_day * 2);
	int i = viTri_dong + viTri_day * p_hienTai->cb.soDong;
	highlight();
	cout << p_hienTai->cb.dsVe.nodes[i].soVe;
	normal();
	do {
		int c = nhanPhim();
		if (c != Enter) {
			gotoxy(x_min + viTri_dong * 4, y_min + viTri_day * 2);
			i = viTri_dong + viTri_day * p_hienTai->cb.soDong;
			if (strcmp(p_hienTai->cb.dsVe.nodes[i].CMND, "trong") != 0) {
				highlightRed();
				cout << p_hienTai->cb.dsVe.nodes[i].soVe;
				normal();
			}
			else {
				normal();
				cout << p_hienTai->cb.dsVe.nodes[i].soVe;
			}
		}
		switch (c)
		{
		case Left: {
			if (viTri_dong == vtDong_min) {
				viTri_dong = vtDong_max;
				if (viTri_day == vtDay_min) {
					viTri_day = vtDay_max;
				}
				else viTri_day--;
			}
			else viTri_dong--;
			break;
		}
		case Right: {
			if (viTri_dong == vtDong_max) {
				viTri_dong = vtDong_min;
				if (viTri_day == vtDay_max) {
					viTri_day = vtDay_min;
				}
				else viTri_day++;
			}
			else viTri_dong++;
			break;
		}
		case Up: {
			if (viTri_day == vtDay_min) {
				viTri_day = vtDay_max;
			}
			else viTri_day--;
			break;
		}
		case Down: {
			if (viTri_day == vtDay_max) {
				viTri_day = vtDay_min;
			}
			else viTri_day++;
			break;
		}
		case Enter: {
			if (strcmp(p_hienTai->cb.dsVe.nodes[i].CMND, "trong") != 0) {
				inVaXoaTBLoi(44);
				break;
			}
			i = viTri_dong + viTri_day * p_hienTai->cb.soDong;
			return i;
			break;
		}
		case ESC: {
			return -1;
		}
		default:
			break;
		}
		gotoxy(x_min + viTri_dong * 4, y_min + viTri_day * 2);
		i = viTri_dong + viTri_day * p_hienTai->cb.soDong;
		highlight();
		cout << p_hienTai->cb.dsVe.nodes[i].soVe;
		normal();
	} while (true);
}

void datVe(nodesCB_PTR& first, nodesHK_PTR& root) {
	//chon chuyen bay de dat ve
chon_chuyen_bay:
	normal();
	clrscr();
	nodesCB_PTR p_hienTai = chonChuyenBayConVe(first);
	if (p_hienTai == NULL) return;
	//nhap so cmnd
nhap_so_cmnd:
	normal();
	clrscr();
	int x = 25;
	int y = 8;
	boxTitle(16);
	boxDieuHuong(15);
	SetColor(Aqua);
	box(x, y + 5, 70, 3);
	SetColor(Black);
	gotoxy(x + 1, y + 6);
	cout << "Nhap so CMND hanh khach: ";
	x = x + strlen("Nhap so CMND hanh khach: ") + 1;
	y += 6;
	char cmnd[11] = ""; //cmnd toi da 10 ky tu
	gotoxy(x, y);
	bool thoat = false;
	do {
		int c = nhanPhim();
		switch (c)
		{
		case Backspace: {
			if (xoaKyTu(cmnd, x, y) == false) {
				break;
			}
			break;
		}
		case Enter: {
			if (strlen(cmnd) == 0) {
				inVaXoaTBLoi(9);
				gotoxy(x, y);
				break;
			}
			if (checkHK_thuocCB(p_hienTai, cmnd) == true) {
				inVaXoaTBLoi(41);
				gotoxy(x + strlen(cmnd), y);
				break;
			}
			//thieu kiem tra hk dat 2 chuyen bay
			thoat = true;
			break;
		}
		case ESC: {
			normal();
			clrscr();
			goto chon_chuyen_bay;
		}
		default:
			if (isNumber(c) == true) {
				if (themKyTu(cmnd, sizeof(cmnd), c) == true) {
					gotoxy(x, y);
					char tmp = cmnd[strlen(cmnd) - 1];
					if (islower(tmp)) {
						cmnd[strlen(cmnd) - 1] = toupper(tmp);
					}
					cout << cmnd;
				}
				else break;
			}
			else break;
			break;
		}
	} while (thoat == false);
	// ket thuc vong lap da nhan duoc so cmnd hanh khach nhap
	// kiem tra hanh khach vs cmnd da ton tai hay chua
	normal();
	clrscr();
	nodesHK_PTR hk = searchNodes_HK(root, cmnd);
	if (hk == NULL) {
		hk = themHanhKhach(root, cmnd);
		if (hk == NULL) {
			goto nhap_so_cmnd;
		}
	}
	else { //in ra khach hang de kiem tra
		showHanhKhach(hk);
		thoat = false;
		do {
			int k = nhanPhim();
			if (k == Enter) {
				thoat = true;
			}
			else if (k == ESC) {
				goto nhap_so_cmnd;
			}
		} while (thoat == false);
	}
	normal();
	clrscr();
	int viTri = chonSoVe(p_hienTai, hk);
	if (viTri != -1) {
		p_hienTai->cb.dsVe.n++;
		strcpy_s(p_hienTai->cb.dsVe.nodes[viTri].CMND, hk->hk.CMND);
		boxDetail(13, 12, 7);
		int k = nhanPhim();
		if (k == 'y' || k == 'Y') {
			goto chon_chuyen_bay;
		}
		else {
			return;
		}
	}
	else goto nhap_so_cmnd;
}

void huyVe(nodesCB_PTR& first, nodesHK_PTR root) {

}

//////////////////////////////Quan ly///////////////////////////////

void capNhatTrangThaiCB_HoanTat(nodesCB_PTR& first) {
	for (nodesCB_PTR p = first; p != NULL; p = p->next) {
		if (p->cb.trangThai == 1 || p->cb.trangThai == 2) {
			if (checkNgayLapChuyenBay(p->cb.ngayGioKH) == false) {
				p->cb.trangThai = 3; //hoan tat
			}
		}
	}
}

void quanLyMayBay(listMB& ds, nodesCB_PTR first) {
	while (true) {
		int c = trangQLMB();
		switch (c)
		{
		case 1: {
			if (isFull(ds)) {
				inVaXoaTBLoi(1);
				break;
			}
			int cont = 1;
			do {
				clrscr();
				cont = themMayBay(ds);
				if (cont == 1) {
					if (isFull(ds)) {
						inVaXoaTBLoi(1);
						normal();
						clrscr();
						break;
					}
				}
				normal();
				clrscr();
			} while (cont);
			break;
		}
		case 2: { //xoa mb
			if (isEmpty(ds)) {
				inVaXoaTBLoi(2);
				break;
			}
			int cont = 1;
			do {
				clrscr();
				cont = timKiem_Xoa_SuaMB(ds, first, 2);
				if (cont == 1) {
					if (isEmpty(ds)) {
						inVaXoaTBLoi(2);
						normal();
						clrscr();
						break;
					}
				}
				normal();
				clrscr();
			} while (cont);
			break;
		}
		case 3: { //sua mb
			if (isEmpty(ds)) {
				inVaXoaTBLoi(3);
				break;
			}
			int cont = 1;
			do {
				clrscr();
				cont = timKiem_Xoa_SuaMB(ds, first, 3);
				normal();
				clrscr();
			} while (cont);
			break;
		}
		case 4: {
			if (isEmpty(ds)) {
				inVaXoaTBLoi(30);
				break;
			}
			clrscr();
			timKiem_Xoa_SuaMB(ds, first, 0);
			normal();
			clrscr();
			break;
		}
		case ESC: {
			return;
		}
		default:
			break;
		}
	}
}

void quanLyChuyenBay(listMB ds, nodesCB_PTR& first) {
	while (true) {
		int c = trangQLCB();
		switch (c)
		{
		case 1: {
			int cont = 1;
			do {
				clrscr();
				cont = themChuyenBay(ds, first);
				normal();
				clrscr();
			} while (cont);
			break;
		}
		case 2: {
			if (isEmpty(first)) {
				inVaXoaTBLoi(31);
				break;
			}
			int cont = 1;
			do {
				clrscr();
				cont = timKiem_Xoa_SuaCB(first, ds, 2);
				if (cont == 1) {
					if (isEmpty(first)) {
						inVaXoaTBLoi(31);
						normal();
						clrscr();
						break;
					}
				}
				normal();
				clrscr();
			} while (cont);
			break;
		}
		case 3: {
			if (isEmpty(first)) {
				inVaXoaTBLoi(31);
				break;
			}
			int cont = 1;
			do {
				clrscr();
				cont = timKiem_Xoa_SuaCB(first, ds, 3);
				if (cont == 1) {
					if (isEmpty(first)) {
						inVaXoaTBLoi(31);
						normal();
						clrscr();
						break;
					}
				}
				normal();
				clrscr();
			} while (cont);
			break;
		}
		case 4: {
			if (isEmpty(first)) {
				inVaXoaTBLoi(31);
				break;
			}
			clrscr();
			huyChuyenBay(first);
			normal();
			clrscr();
			break;
			break;
		}
		case 5: {
			if (isEmpty(first)) {
				inVaXoaTBLoi(31);
				break;
			}
			clrscr();
			timKiem_Xoa_SuaCB(first, ds, 0);
			normal();
			clrscr();
			break;
		}
		case ESC: {
			return;
		}
		default:
			break;
		}
	}
}

void quanLyDatHuyVe(nodesCB_PTR& first, nodesHK_PTR& root) {
	while (true) {
		int c = trangDvHv();
		switch (c)
		{
		case 1: {
			if (isEmpty(first)) {
				inVaXoaTBLoi(31);
				break;
			}
			clrscr();
			datVe(first, root);
			normal();
			clrscr();
			break;
		}
		case 2: {
			if (isEmpty(first)) {
				inVaXoaTBLoi(31);
				break;
			}
			clrscr();
			huyVe(first, root);
			normal();
			clrscr();
			break;
		}
		case 3: {
			break;
		}
		case ESC: {
			return;
		}
		default:
			break;
		}
	}
}

void ThongKe(listMB ds, nodesCB_PTR first, nodesHK_PTR root) {
	while (true) {
		int c = trangThongKe();
		switch (c)
		{
		case 1: {

			break;
		}
		case 2: {
			break;
		}
		case 3: {
			break;
		}
		case 4: {
			break;
		}
		case ESC: {
			return;
		}
		default:
			break;
		}
	}
}

void chonChucNang(listMB& ds, nodesCB_PTR& first, nodesHK_PTR& root) {
	while (true) {
		int c = TrangChu();
		switch (c)
		{
		case 1: {
			capNhatTrangThaiCB_HoanTat(first);
			clrscr();
			quanLyMayBay(ds, first);
			normal();
			clrscr();
			break;
		}
		case 2: {
			capNhatTrangThaiCB_HoanTat(first);
			clrscr();
			quanLyChuyenBay(ds, first);
			normal();
			clrscr();
			break;
		}
		case 3: {
			capNhatTrangThaiCB_HoanTat(first);
			clrscr();
			quanLyDatHuyVe(first, root);
			normal();
			clrscr();
			break;
		}
		case 4: {
			capNhatTrangThaiCB_HoanTat(first);
			clrscr();
			ThongKe(ds, first, root);
			normal();
			clrscr();
			break;
		}
		case 5: {
			clrscr();
			gotoxy(cot, dong);
			cout << "Ban co chac muon thoat chuong trinh ?(y/n)";
			char a = _getch();
			if (a == char('y') || a == char('Y')) {
				return;
			}
			clrscr();
			break;
		}
		case ESC: {
			normal();
			clrscr();
			gotoxy(cot, dong);
			cout << "Ban co chac muon thoat chuong trinh ?(y/n)";
			char a = _getch();
			if (a == char('y') || a == char('Y')) {
				return;
			}
			clrscr();
			break;
		}
		default:
			break;
		}
	}
}

int main() {
	system("color F0");
	char file_MB[] = "maybay.txt";
	char file_CB[] = "chuyenbay.txt";
	char file_HK[] = "hanhkhach.txt";
	listMB ds;
	khoiTao_MB(ds);
	openFile_MB(ds, file_MB);
	nodesCB_PTR first;
	khoiTaoDS_CB(first);
	openFile_CB(first, file_CB);
	nodesHK_PTR root;
	khoiTao_HK(root);
	openFile_HK(root, file_HK);
	capNhatTrangThaiCB_HoanTat(first);

	setFullScreen();
	chonChucNang(ds, first, root);

	saveFile_MB(ds, file_MB);
	saveFile_CB(first, file_CB);
	saveFile_HK(root, file_HK);
	clearList_MB(ds);
	clearList_CB(first);
	clearTree_HK(root); //thieu
	return 0;
}