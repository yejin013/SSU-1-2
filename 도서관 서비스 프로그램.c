/*
2018 2학기 프로그래밍 응용 및 실습
프로젝트 : 도서관 서비스 프로그램

감예지, 남지원, 박수현, 이수민, 채예진
 */

//헤더파일
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h> //sleep 함수

FILE *fp_c, *fp_bk, *fp_bw;

//구조체
typedef struct Client{
  int s_num;
  char *passwd, *c_name, *address, *p_num;
  struct Client *Next_c;
}Client;

typedef struct Book{
  int b_num;
  unsigned long long isbn;
  char *b_name, *publisher, *author, *location, ox;
  struct Book *Next_bk;
}Book;

typedef struct Borrow{
  int snum, bnum;
  time_t borrow_b, return_b;
  struct Borrow *Next_bw;
}Borrow;

//함수원형
void c_read(); //client 파일 읽어들이는 함수
void bk_read(); //book 파일 읽어들이는 함수
void bw_read(); //borrow 파일 읽어들이는 함수
void signup(); //회원가입
int login(); //로그인
int usermenu(); //회원용 메뉴
int adminmenu(); //관리자용 메뉴
void searchmenu(int n); //도서 검색 함수
void searchclient(void); //회원 목록 출력 함수
void booksearch(void);//책 제목 검색 후 출력
void publishersearch(void);//출판사명 검색
void ISBNsearch(void);//ISBN 검색
void authorsearch(void);//저자명 검색
void allsearch(void);//전체 검색
void borrowbook(Book *);//몇권 중에 몇권 대출중인지
int joongbok(Book *);//중복여부
void findborrowlist(); //내 대여 목록
void client_modify();//회원수정
void client_delete();//회원삭제
void book_add();//도서 추가
void bnumprint();//도서 삭제시 검색 결과에서 이름이 중복되는 도서들의 도서번호와 삭제 가능 여부를 찾아서 출력해주는 함수
void findbook();//책 제목 검색
void findisbn();//isbn 검색
void deletebook();//도서 삭제
void cnumprint();//도서 대여시 검색 결과에서 isbn이 중복되는 도서들의 도서번호와 대여 가능 여부를 찾아서 일괄 출력해주는 함수
void dybooksearch();//대여할 때 책 제목으로 책 검색 후 대여해주는 함수
void dyISBNsearch();//대여할 때 책 ISBN으로 책 검색 후 대녀해주는 함수
int bkjoongbok(int);//대여가능 여부를 알아보는 함수
void bk_check();//책을 대여할 때 책의 대여가능여부를 바꾸어주는 함수
void deyeo();//borrow1파일에 대여 정보를 입력해주는 함수
void printdeyeo();
void returnbook(); //도서 반납
void namesearch();//회원이름으로 회원 검색
void studentidsearch();//학번으로 회원 검색
void allclientsearch();//전체 회원 출력
void cfile_manage(); //client 파일에 저장
void bkfile_manage(); //book 파일에 저장
void bwfile_manage(); //borrow 파일에 저장
void cfree_node(); //client 관련 연결리스트 해제
void bkfree_node(); //book 관련 연결리스트 해제
void bwfree_node(); //borrow 관련 연결리스트 해제

Client *c_head=NULL, *c_prev=NULL, *c_current=NULL, *c_tail=NULL, *c_get=NULL;
Book *bk_head=NULL, *bk_prev=NULL, *bk_current=NULL, *bk_tail=NULL, *bk_get=NULL;
Borrow *bw_head=NULL, *bw_prev=NULL, *bw_current=NULL, *bw_tail=NULL, *bw_get=NULL;
//전역변수
int stuID = 0;

int mainmenu(void){ //메인메뉴
  int type;
  _Bool loginOK;

  system("clear");
  printf(">> 도서관 서비스 <<\n");
  printf("1. 회원 가입      2. 로그인      3. 프로그램 종료\n\n");
  printf("번호를 선택하세요: ");
  scanf("%d", &type);

  switch (type) {
    case 1 :
    signup();
    system("clear");
    mainmenu(); // 회원가입 후 메인메뉴 복귀

    case 2 :
    loginOK = login();
    if(loginOK){
      usermenu();
    }
    else{
      Sleep(1000);
      system("clear");
      mainmenu();
    }

    case 3:
    cfree_node();
    bkfree_node();
    bwfree_node();
    exit(0);
    break;

    default:
    printf("잘못입력하셨습니다.\n");
    Sleep(1000);
    mainmenu();
    break;
  };

  return 0;
}

void c_read(void){
  int nums;
  char pass[100], namec[100], addr[100], pnum[100];

  c_current = c_head -> Next_c;
  c_prev = c_head;

  while(feof(fp_c) != EOF){

    if(fscanf(fp_c, "%d|%[^|]|%[^|]|%[^|]|%s\n", &nums, pass, namec, addr, pnum) == EOF)
      break;

      c_prev=c_head;
      c_current=c_head->Next_c;
      while(c_current!=c_tail && c_current->s_num < nums)
      {
        c_prev=c_current;
        c_current=c_current->Next_c;
      }

    c_get = (Client *)malloc(sizeof(Client));

    c_get -> s_num = nums;

    c_get -> Next_c = c_current;
    c_prev -> Next_c = c_get;

    c_get -> passwd = (char *)malloc(strlen(pass));
    strcpy(c_get -> passwd, pass);

    c_get -> c_name = (char *)malloc(strlen(namec));
    strcpy(c_get -> c_name, namec);

    c_get -> address = (char *)malloc(strlen(addr));
    strcpy(c_get -> address, addr);

    c_get -> p_num = (char *)malloc(strlen(pnum));
    strcpy(c_get -> p_num, pnum);
  }
  fclose(fp_c);
}

void bk_read(void){
  int nums;
  unsigned long long ib;
  char nameb[100], pub[100], auth[100], loca[100], yn;

  bk_current = bk_head -> Next_bk;
  bk_prev = bk_head;

  while(feof(fp_bk) != EOF){

    if(fscanf(fp_bk, "%d|%[^|]|%[^|]|%[^|]|%llu|%[^|]|%c\n", &nums, nameb, pub, auth, &ib, loca, &yn) == EOF)
      break;

      bk_prev=bk_head;
      bk_current=bk_head->Next_bk;
      while(bk_current!=bk_tail && bk_current->isbn < ib)
      {
        bk_prev=bk_current;
        bk_current=bk_current->Next_bk;
      }

    bk_get = (Book *)malloc(sizeof(Book));

    bk_get -> b_num = nums;

    if(bk_current -> isbn == ib){
      if(bk_current -> b_num > bk_get -> b_num){
        bk_prev -> Next_bk = bk_get;
        bk_get -> Next_bk = bk_current;
      }
      else{
        bk_prev = bk_current;
        bk_current = bk_current -> Next_bk;
        bk_prev -> Next_bk = bk_get;
        bk_get -> Next_bk = bk_current;
      }
    }
    else{
      bk_get -> Next_bk = bk_current;
      bk_prev -> Next_bk = bk_get;
    }

    bk_get -> b_name = (char *)malloc(strlen(nameb));
    strcpy(bk_get -> b_name, nameb);

    bk_get -> publisher = (char *)malloc(strlen(pub));
    strcpy(bk_get -> publisher, pub);

    bk_get -> author = (char *)malloc(strlen(auth));
    strcpy(bk_get -> author, auth);

    bk_get -> isbn = ib;

    bk_get -> location = (char *)malloc(strlen(loca));
    strcpy(bk_get -> location, loca);

    bk_get -> ox = yn;
  }
  fclose(fp_bk);
}

void bw_read(void){
  int nums, numb;
  time_t borrowb, returnb;

  bw_current = bw_head -> Next_bw;
  bw_prev = bw_head;

  while(feof(fp_bw) != EOF){

    if(fscanf(fp_bw, "%d|%d|%ld|%ld\n", &nums, &numb, &borrowb, &returnb) == EOF)
      break;

    bw_prev = bw_head;
    bw_current = bw_head -> Next_bw;

    while(bw_current != bw_tail){
      bw_prev = bw_current;
      bw_current = bw_current -> Next_bw;
    }

    bw_get = (Borrow *)malloc(sizeof(Borrow));
    bw_get -> snum = nums;

    bw_get -> Next_bw = bw_current;
    bw_prev -> Next_bw = bw_get;

    bw_get -> bnum = numb;
    bw_get -> borrow_b = borrowb;
    bw_get -> return_b = returnb;
    bw_current = bw_get;
  }
}

void signup(void){ // =client 파일에 새로운 회원 추가하는 기능
  printf("\n>> 회원 가입 <<\n");
  printf("학번, 비밀번호, 이름, 주소, 전화번호를 입력하세요.\n\n\n");
  char input[100];
  int num;

  c_get = (Client *)malloc(sizeof(Client));

  printf("학번 : ");
  scanf("%d", &num);
  getchar();

  c_prev=c_head;
  c_current=c_head->Next_c;
  while(c_current!=c_tail && ((c_current->s_num) < num)) //name이 작으면 음수, 크면 양수, 같으면 0   name이 ㄱ, arr이 ㅎ이면 음수
  {
      c_prev=c_current;
      c_current=c_current->Next_c;
  }
    if(c_current -> s_num == num){
      printf("\n이미 존재하는 학번입니다.\n"); // 이미 존재하는 학번
      Sleep(10000);
      mainmenu();
  }

	(c_get->s_num) = num;

  c_get->Next_c = c_current;
  c_prev->Next_c = c_get;

  printf("비밀번호 : ");
  gets(input);
  c_get->passwd = (char *)malloc(strlen(input));
  strcpy(c_get->passwd,input);
  printf("이름 : ");
  gets(input);
  c_get-> c_name = (char *)malloc(strlen(input));
  strcpy(c_get->c_name, input);
  printf("주소 : ");
  gets(input);
  c_get->address = (char *)malloc(strlen(input));
  strcpy(c_get->address, input);
  printf("전화번호 : ");
  gets(input);
  c_get-> p_num= (char *)malloc(strlen(input));
  strcpy(c_get->p_num, input);
  printf("\n");
  printf("회원가입이 되셨습니다.\n");
  Sleep(2000);

  cfile_manage(); //파일 내용 저장
}

int login(void){
  char ID[10] = { }, password[15];
  printf("\n>> 로그인 <<\n");
  printf("학번 : ");
  scanf("%s", ID);
  if(strcmp(ID, "admin")==0){
    printf("비밀번호 : ");
    scanf("%s",password);
    if(strcmp(password,"lib_admin7")==0){
      adminmenu();
    }
    else{
      printf("\n로그인 실패\n");
      return 0;
    }
  }
       //만일 admin이 입력되면 adminmenu로
  else{
    stuID = atoi(ID);

    c_get = (Client *)malloc(sizeof(Client));

    c_prev=c_head;
    c_current=c_head->Next_c;
    while(c_current!=c_tail && ((c_current->s_num) < stuID)) //name이 작으면 음수, 크면 양수, 같으면 0   name이 ㄱ, arr이 ㅎ이면 음수
    {
        c_prev=c_current;
        c_current=c_current->Next_c;
    }
    if(c_current ->s_num == stuID){
      printf("비밀번호 : ");
      scanf("%s", password);
      if(strcmp(c_current -> passwd, password) == 0)
        return 1;
      else{
        printf("\n로그인 실패\n");
        return 0;
      }
    }
    else{
      printf("\n로그인 실패\n");
      return 0;
    }
  }
}

int usermenu(void){ //#1 회원용 메뉴
  int num;
  printf("\n>> 회원메뉴 <<\n");
  printf("1. 도서 검색           2. 내 대여 목록\n3. 개인정보 수정       4. 회원 탈퇴\n5. 로그아웃            6. 프로그램 종료\n\n");
  printf("번호를 선택하세요: ");
  scanf("%d", &num);
  getchar();

  switch (num) {
    case 1:
    searchmenu(1);
    break;

    case 2:
    printf("\n\n>> 내 대여 목록 <<\n");
    findborrowlist(); //회원의 대여 목록 출력 - 나중에 한번 더 써서 함수로 만듬
    usermenu();
    break;

    case 3:
    client_modify();
    usermenu();
    break;
    //학번과 이름을 제외한 모든 정보를 수정할 수 있다.
    //수정 후에는 그 결과를 clinet 파일에 저장해야 한다.

    case 4:
    client_delete();
    mainmenu();
    break;

    case 5:
    printf("로그아웃되셨습니다.\n");
    Sleep(1000);
    stuID = 0;
    system("clear");
    mainmenu(); // 메인메뉴로
    break;

    case 6:
    cfree_node();
    bkfree_node();
    bwfree_node();
    exit(0);

    default:
      printf("잘못입력하셨습니다.\n");
      usermenu();
  }
}

int adminmenu(void){ //#2 관리자 메뉴
  int num=0, n=0;

  printf("\n>> 관리자 메뉴 <<\n");
  printf("1. 도서 등록           2. 도서 삭제\n3. 도서 대여           4. 도서 반납\n5. 도서 검색           6. 회원 목록\n7. 로그아웃            8. 프로그램 종료\n\n");
  printf("번호를 선택하세요: ");
  scanf("%d", &num);
  getchar();

  switch (num) {
    case 1:
    book_add();
    adminmenu();
    break;

    case 2:
    printf("\n>> 도서 삭제 <<\n");
    printf("1. 도서명 검색       2. ISBN 검색\n\n");
    printf("검색 번호를 입력하세요 : ");
    scanf("%d",&n);
    getchar();
    if(n == 1)
      findbook(); //도서명 검색
    else if(n == 2)
      findisbn(); //ISBN 검색
    else
      printf("잘못 입력하셨습니다.");
 //도서 삭제 서비스
    adminmenu();
    break;

    case 3:
    printf("\n>> 도서 대여 <<\n");
    printf("1. 도서명 검색       2. ISBN 검색\n\n");
    printf("검색 번호를 입력하세요 : ");
    scanf("%d",&n);
    getchar();
    switch(n){
      case 1:
          dybooksearch();
          break;
      case 2:
          dyISBNsearch();
          break;
      default:
        printf("잘못입력하셨습니다.\n");
        adminmenu();
        break;
      }
    break;

    case 4: //도서 반납
    printf("\n학번을 입력하세요 : ");
    scanf("%d", &stuID);
    printf("\n\n>> 회원의 대여 목록 <<\n");
    findborrowlist(); // 대여 목록 출력(위에서 사용한 적 있음)
    returnbook(); // 도서 반납
    adminmenu();
    break;

    case 5: //도서 검색
    searchmenu(0);
    break;

    case 6: //회원 목록
    searchclient();
    break;

    case 7: // 로그아웃
    printf("로그아웃되셨습니다.\n");
    Sleep(1000);
    stuID = 0;
    system("clear");
    mainmenu();
    break;

    case 8: // 프로그램 종료
    cfree_node();
    bkfree_node();
    bwfree_node();
    exit(0);

    default:
    printf("잘못입력하셨습니다.\n");
    adminmenu();
  }
}

void searchmenu(int n){
  printf("\n>> 도서 검색 <<\n");
  printf("1. 도서명 검색    2. 출판사 검색\n");
  printf("3. ISBN 검색    4. 저자명 검색\n");
  printf("5. 전체 검색    6. 이전메뉴\n\n");

  int num;
  printf("번호를 선택하세요: ");
  scanf("%d",&num);
  getchar();
  switch(num)
  {
          case 1:
          booksearch();
          searchmenu(n);
          break;
          case 2:
          publishersearch();
          searchmenu(n);
          break;
          case 3:
          ISBNsearch();
          searchmenu(n);
          break;
          case 4:
          authorsearch();
          searchmenu(n);
          break;
          case 5:
          allsearch();
          searchmenu(n);
          break;
          case 6:
          if(n == 1){
            usermenu();
            break;
          }
          else{
            adminmenu();
            break;
          }
          default:
          printf("잘못입력하셨습니다.\n");
          break;
  }
}

void searchclient(){
  int n;
  printf("\n>> 회원 목록 <<\n");
  printf("1. 이름 검색		2. 학번 검색\n3. 전체 검색		4. 이전 메뉴\n\n");
  printf("검색 번호를 입력하세요 : ");
  scanf("%d",&n);
  getchar();
  switch(n)
  {
  case 1:
  namesearch();
  searchclient();
  break;

  case 2:
  studentidsearch();
  searchclient();
  break;

  case 3:
  allclientsearch();
  searchclient();
  break;

  case 4:
  adminmenu();
  break;

  default:
  printf("잘못입력하셨습니다.\n");
  break;
  }
}

void booksearch(void)
{
        char name[100];
        printf("\n도서명을 입력하세요 : ");
        gets(name);
        printf("\n\n>>검색결과<<");
        bk_prev= bk_head;
        bk_current = bk_prev -> Next_bk;

        while(bk_current != bk_tail){
             if(strstr(bk_current -> b_name ,name)){
               if(joongbok(bk_current)==0){
                  printf("\n도서명: %s\n",bk_current -> b_name);
                  printf("출판사: %s\n",bk_current -> publisher);
                  printf("저자명: %s\n",bk_current -> author);
                  printf("ISBN: %13llu\n",bk_current -> isbn);
                  printf("소장처: %s\n",bk_current -> location);
                  printf("대여가능 여부: ");
                  borrowbook(bk_current);
                  printf("\n");
                }
              }

              bk_current = bk_current -> Next_bk;
       }
       printf("\n** Y는 대여가능, N은 대여불가를 의미\n");
       printf("** (x/y) : (대여된 총 권수 / 보유하고 있는 총 권수)\n");
}

void publishersearch(void)
{
       char name[100];
       printf("\n출판사를 입력하세요 : ");
       scanf("%[^\n]",name);
       getchar();
       printf("\n\n>>검색결과<<");

       bk_prev= bk_head;
       bk_current = bk_prev -> Next_bk;

       while(1){

               if(bk_current == bk_tail)
                     break;

               if(strstr(bk_current -> publisher ,name)){
                 if(joongbok(bk_current)==0){
                       printf("\n도서명: %s\n",bk_current -> b_name);
                       printf("출판사: %s\n",bk_current -> publisher);
                       printf("저자명: %s\n",bk_current -> author);
                       printf("ISBN: %llu\n",bk_current -> isbn);
                       printf("소장처: %s\n",bk_current -> location);
                       printf("대여가능 여부: ");
                       borrowbook(bk_current);
                       printf("\n");
                   }
                 }

           bk_current = bk_current -> Next_bk;
      }
       printf("\n** Y는 대여가능, N은 대여불가를 의미\n");
       printf("** (x/y) : (대여된 총 권수 / 보유하고 있는 총 권수)\n");
}

void ISBNsearch(void)
{
        unsigned long long ISBN;
        printf("\nISBN을 입력하세요 : ");
        scanf("%13llu",&ISBN);
        getchar();
        printf("\n\n>>검색결과<<");

        bk_prev= bk_head;
        bk_current = bk_prev -> Next_bk;

        while(1){

                if(bk_current == bk_tail)
                        break;

                if(bk_current -> isbn == ISBN){
                  if(joongbok(bk_current)==0){
                        printf("\n도서명: %s\n",bk_current -> b_name);
                        printf("출판사: %s\n",bk_current -> publisher);
                        printf("저자명: %s\n",bk_current -> author);
                        printf("ISBN: %13llu\n",bk_current -> isbn);
                        printf("소장처: %s\n",bk_current -> location);
                        printf("대여가능 여부: ");
                        borrowbook(bk_current);
                        printf("\n");
                  }
                }

                bk_current = bk_current -> Next_bk;

        }
        printf("\n** Y는 대여가능, N은 대여불가를 의미\n");
        printf("** (x/y) : (대여된 총 권수 / 보유하고 있는 총 권수)\n");
}

void authorsearch(void)
{
        char name[100];
        printf("\n저자명을 입력하세요 : ");
        scanf("%[^\n]",name);
        getchar();
        printf("\n\n>>검색결과<<");

        bk_prev= bk_head;
        bk_current = bk_prev -> Next_bk;

        while(1){

                if(bk_current == bk_tail)
                        break;

                if(strstr(bk_current -> author ,name)){
                  if(joongbok(bk_current)==0){
                  printf("\n도서명: %s\n",bk_current -> b_name);
                  printf("출판사: %s\n",bk_current -> publisher);
                  printf("저자명: %s\n",bk_current -> author);
                  printf("ISBN: %13llu\n",bk_current -> isbn);
                  printf("소장처: %s\n",bk_current -> location);
                  printf("대여가능 여부: ");
                  borrowbook(bk_current);
                  printf("\n");
                }
              }

          bk_current = bk_current -> Next_bk;

  }
  printf("\n** Y는 대여가능, N은 대여불가를 의미\n");
  printf("** (x/y) : (대여된 총 권수 / 보유하고 있는 총 권수)\n");
}

void allsearch(void)
{
  bk_prev= bk_head;
  bk_current = bk_prev -> Next_bk;
  printf("\n\n>>검색결과<<");


  while(1){

          if(bk_current == bk_tail)
                  break;

            if(joongbok(bk_current)==0){
                  printf("\n도서명: %s\n",bk_current -> b_name);
                  printf("출판사: %s\n",bk_current -> publisher);
                  printf("저자명: %s\n",bk_current -> author);
                  printf("ISBN: %llu\n",bk_current -> isbn);
                  printf("소장처: %s\n",bk_current -> location);
                  printf("대여가능 여부: ");
                  borrowbook(bk_current);
                  printf("\n");
              }

          bk_current = bk_current -> Next_bk;
  }
  printf("\n** Y는 대여가능, N은 대여불가를 의미\n");
  printf("** (x/y) : (대여된 총 권수 / 보유하고 있는 총 권수)\n");
}

void borrowbook(Book *bk_current)
{
  Book *bk_bcurrent = NULL;
  int sum=0;
  int a=0;
  bk_bcurrent = (Book *)malloc(sizeof(Book *));
  bk_bcurrent= bk_head -> Next_bk;

  while(bk_bcurrent != bk_tail){
    if(bk_bcurrent -> isbn== bk_current -> isbn){
      sum++;
      if(bk_bcurrent -> ox == 'N'){
      a++;
    }
  }
  bk_bcurrent=bk_bcurrent -> Next_bk;
  }

  if(a == sum)
  {
    printf("N ");
  }
  else
  {
    printf("Y ");
  }
  printf("(%d/%d)",a,sum);
}

int joongbok(Book *bk_current)
{
  Book *bk_jcurrent=NULL;
  bk_jcurrent = (Book *)malloc(sizeof(Book));
  bk_jcurrent = bk_head -> Next_bk;
  int a = bk_current -> b_num;
  while(1)
  {
    if(bk_jcurrent->isbn==bk_current -> isbn){
      if(bk_jcurrent -> b_num < bk_current ->b_num ){
        a = bk_jcurrent-> b_num;
      }
    }
    bk_jcurrent= bk_jcurrent -> Next_bk;
    if(bk_jcurrent==bk_tail){
      break;
    }
  }
  return (bk_current -> b_num - a);
}

void findborrowlist(){
  int booknum[100] = {0};
  int cnt = 0;
  bw_prev = bw_head;
	bw_current = bw_head -> Next_bw;

	while(bw_current != bw_tail)
	{
    if(bw_current -> snum == stuID){
      booknum[cnt] = bw_current -> bnum;
      cnt++;
    }
    bw_prev = bw_current;
		bw_current = bw_current -> Next_bw;
	}

  bk_prev= bk_head;
  bk_current = bk_prev -> Next_bk;

  while(bk_current != bk_tail)
  {
    for(int i = 0 ; i <= cnt ; i++){
      if(bk_current -> b_num == booknum[i]){
        printf("도서번호 : %d\n", bk_current -> b_num);
        printf("도서명 : %s\n", bk_current -> b_name);
        {
         bw_prev = bw_head;
    	   bw_current = bw_head -> Next_bw;

    	   while(bw_current != bw_tail)
    	   {
            if(bw_current -> bnum == bk_current -> b_num){
              break;
            }
            bw_prev = bw_current;
    		    bw_current = bw_current -> Next_bw;
    	   }
          printdeyeo();
          printf("\n");
        }
      }
    }
    bk_prev=bk_current;
    bk_current=bk_current->Next_bk;
  }
}

void printdeyeo(){
  struct tm *t=NULL,*nt=NULL;
  const char *day[]={"일", "월", "화","수", "목", "금", "토"};

  t = localtime(&(bw_current -> borrow_b));
  printf("대여일자: %d년 %d월 %d일 %s요일\n",t ->tm_year+1900,t->tm_mon+1,t->tm_mday,day[t->tm_wday]);

  nt = localtime(&(bw_current -> return_b));
  printf("반납일자: %d년 %d월 %d일 %s요일\n", nt ->tm_year+1900, nt->tm_mon+1, nt->tm_mday, day[nt->tm_wday]);
}

void client_modify(){
  printf("\n>> 개인정보 수정 <<\n");
  char input[100];

  c_prev = c_head;
  c_current = c_head -> Next_c;

  while(c_current != c_tail && c_current -> s_num < stuID)
  {
    c_prev = c_current;
    c_current = c_current -> Next_c;
  }
  strcpy(input, c_current->c_name);

    c_prev -> Next_c = c_current -> Next_c;
    free(c_current);
    c_current = c_prev -> Next_c;

    c_get = (Client *)malloc(sizeof(Client));

    c_get->s_num = stuID;
    c_get -> Next_c = c_current;
    c_prev -> Next_c = c_get;

    strcpy(c_get->c_name, input);

    printf("비밀번호 : ");
    gets(input);
    c_get->passwd = (char *)malloc(strlen(input));
    strcpy(c_get->passwd,input);
    printf("주소 : ");
    gets(input);
    c_get->address = (char *)malloc(strlen(input));
    strcpy(c_get->address, input);
    printf("전화번호 : ");
    gets(input);
    c_get-> p_num= (char *)malloc(strlen(input));
    strcpy(c_get->p_num, input);

  printf("\n");
  printf("정보가 수정되었습니다.\n");

  cfile_manage();
}

void client_delete(){
  printf("\n>> 회원 탈퇴 <<\n");

	c_prev = c_head;
	c_current = c_head -> Next_c;

	while(c_current != c_tail && c_current -> s_num < stuID)
	{
    c_prev = c_current;
		c_current = c_current -> Next_c;
	}

  bw_prev = bw_head;
  bw_current = bw_head -> Next_bw;

  while(bw_current != bw_tail && bw_current -> snum < stuID)
  {
    bw_prev = bw_current;
    bw_current = bw_current -> Next_bw;
  }

  if(c_current -> s_num == bw_current -> snum){
    printf("\n회원탈퇴를 실패하였습니다.\n\n");
    usermenu();
  }
  else{
    c_prev -> Next_c = c_current -> Next_c;
    free(c_current);
    c_current = NULL;
    c_current = c_prev -> Next_c;

    cfile_manage();

	   printf("\n");
	   printf("정보가 삭제되었습니다.\n");
     Sleep(1000);
   }
}

void book_add(void){ // book 파일에 새로운 도서 추가
  printf("\n>> 도서 등록 <<\n");
  char input[100];
  unsigned long long inputld;
  char yn;
  int num = 0, cnt = 0;

  bk_get = (Book *)malloc(sizeof(Book));

  bk_prev=bk_head;
  bk_current=bk_head->Next_bk;
  while(bk_current!=bk_tail) //끝까지 이동
  {
    if(bk_current -> b_num > num)
      num = bk_current -> b_num;
      bk_prev=bk_current;
      bk_current=bk_current->Next_bk;
  }

	(bk_get->b_num) = num + 1;

  printf("도서명 : ");
  gets(input);
  bk_get->b_name = (char *)malloc(strlen(input));
  strcpy(bk_get->b_name,input);
  printf("출판사 : ");
  gets(input);
  bk_get-> publisher = (char *)malloc(strlen(input));
  strcpy(bk_get->publisher, input);
  printf("저자명 : ");
  gets(input);
  bk_get->author = (char *)malloc(strlen(input));
  strcpy(bk_get->author, input);
  printf("ISBN : ");
  scanf("%llu", &inputld);
  getchar();
  bk_get->isbn = inputld;
  printf("소장처 : ");
  gets(input);
  bk_get-> location= (char *)malloc(strlen(input));
  strcpy(bk_get->location, input);
  bk_get->ox = 'Y';
  printf("\n");
  printf("자동 입력 사항\n\n");
  printf("대여 가능 여부 : %c\n", bk_get -> ox);
  printf("도서 번호 : %d\n", bk_get->b_num);
  printf("\n등록하시겠습니까? ");
  scanf("%c", &yn);
  if(yn == 'Y'){
    bk_prev=bk_head;
    bk_current=bk_head->Next_bk;
    while(bk_current!=bk_tail) //끝까지 이동
    {
        if(bk_get -> isbn < bk_current -> isbn){
          bk_prev -> Next_bk = bk_get;
          bk_get -> Next_bk = bk_current;
          cnt++;
          break;
        }
        else{
          bk_prev=bk_current;
          bk_current=bk_current->Next_bk;
        }
    }
    if(bk_current == bk_tail){
      if(cnt == 0){
        bk_prev -> Next_bk = bk_get;
        bk_get -> Next_bk = bk_current;
      }
    }
    bkfile_manage(); //파일 내용 저장
  }
  else if(yn == 'N'){
    adminmenu();
  }
}

void bnumprint(void){
  Book *bk_ccurrent = NULL;
  bk_ccurrent = (Book *)malloc(sizeof(Book *));
  int n[100]={0};
  char ox[100]={ };
  int i=0;

  bk_ccurrent= bk_head -> Next_bk;

  while(bk_ccurrent != bk_tail){

    if(bk_ccurrent -> isbn==bk_current -> isbn){
        n[i]=bk_ccurrent ->b_num;
        ox[i]=bk_ccurrent -> ox;
        i++;
    }

    bk_ccurrent=bk_ccurrent -> Next_bk;
  }

  printf("도서번호: ");
  for(int j=0;j<i;j++){
    printf("%d(삭제 가능 여부:%c)",n[j],ox[j]);
    if(i!=j+1){ printf(", ");}
  }
}

void findbook(){
  char name[100];
  printf("\n도서명을 입력하세요 : ");
  gets(name);

  bk_prev= bk_head;
  bk_current = bk_prev -> Next_bk;

  printf("\n\n>> 검색 결과 <<\n");

  while(bk_current != bk_tail){
       if(strstr(bk_current -> b_name ,name)){
         if(joongbok(bk_current)==0){
            bnumprint();
            printf("\n도서명: %s\n",bk_current -> b_name);
            printf("출판사: %s\n",bk_current -> publisher);
            printf("저자명: %s\n",bk_current -> author);
            printf("ISBN: %13llu\n",bk_current -> isbn);
            printf("소장처: %s\n\n",bk_current -> location);
        }
      }

        bk_current = bk_current -> Next_bk;
}
  deletebook();
}

void findisbn(){
  unsigned long long ib;
  int cnt = 0;
  printf("ISBN을 입력하세요 : ");
  scanf("%llu", &ib);
  getchar();

  bk_prev= bk_head;
  bk_current = bk_prev -> Next_bk;

  printf("\n\n>> 검색 결과 <<\n");

  while(bk_current != bk_tail){
       if(bk_current -> isbn == ib){
         if(joongbok(bk_current)==0){
            bnumprint();
            printf("\n도서명: %s\n",bk_current -> b_name);
            printf("출판사: %s\n",bk_current -> publisher);
            printf("저자명: %s\n",bk_current -> author);
            printf("ISBN: %13llu\n",bk_current -> isbn);
            printf("소장처: %s\n\n",bk_current -> location);

        }
      }

        bk_current = bk_current -> Next_bk;

    }
  deletebook();
}

void deletebook(){
  int d_num, cnt = 0;
  printf("\n삭제할 도서의 번호를 입력하세요 : ");
  scanf("%d",&d_num);
  getchar();

  bk_prev= bk_head;
  bk_current = bk_prev -> Next_bk;

  while(bk_current != bk_tail){
       if(bk_current -> b_num == d_num){
         break;
         cnt++;
        }
        bk_prev = bk_current;
        bk_current = bk_current -> Next_bk;
  }
  if(bk_current == bk_tail){
    if(cnt == 0){
      printf("\n입력된 도서번호와 일치하는 도서가 없습니다.\n");
      adminmenu();
    }
  }

  if(bk_current -> ox == 'N')
  {
    printf("\n이 도서는 삭제할 수 없습니다.\n");
    return;
  }
  else if(bk_current -> b_num == d_num){

    bk_prev -> Next_bk = bk_current -> Next_bk;

    free(bk_current);

    bk_current = bk_prev -> Next_bk;

	   printf("\n");
	   printf("도서가 삭제되었습니다.\n");

	   bkfile_manage();
  }
}
void cnumprint(void){
  Book *bk_ccurrent = NULL;
  bk_ccurrent = (Book *)malloc(sizeof(Book *));
  int n[100]={0};
  char ox[100]={ };
  int i=0;

  bk_ccurrent= bk_head -> Next_bk;

  while(bk_ccurrent != bk_tail){
    if(bk_ccurrent -> isbn==bk_current -> isbn){
        n[i]=bk_ccurrent ->b_num;
        ox[i]=bk_ccurrent -> ox;
        i++;
    }
    bk_ccurrent=bk_ccurrent -> Next_bk;
  }

  printf("도서번호: ");
  for(int j=0;j<i;j++){
    printf("%d(대여 가능 여부:%c)",n[j],ox[j]);
    if(i!=j+1){ printf(", ");}
  }
}
void dybooksearch(void)
{
        char name[100];
        int cnt = 0;
        printf("도서명을 입력하세요 : ");
        scanf("%[^\n]",name);
        getchar();
        bk_prev= bk_head;
        bk_current = bk_prev -> Next_bk;
        printf("\n\n>>검색 결과<<\n");
        while(1){

             if(bk_current == bk_tail)
               break;

             if(strstr(bk_current -> b_name ,name)){
               if(joongbok(bk_current)==0){
                  cnumprint();
                  printf("\n도서명 : %s\n",bk_current -> b_name);
                  printf("출판사 : %s\n",bk_current -> publisher);
                  printf("저자명 : %s\n",bk_current -> author);
                  printf("ISBN : %13llu\n",bk_current -> isbn);
                  printf("소장처 : %s\n\n\n",bk_current -> location);
                  cnt++;
              }
            }
              bk_current = bk_current -> Next_bk;
       }
       if(bk_current == bk_tail){
         if(cnt == 0){
           printf("도서를 찾을 수 없습니다.\n\n\n");
           dybooksearch();
         }
       }

       bw_prev = bw_head;
       bw_current = bw_head -> Next_bw;
       while(bw_current != bw_tail){
         bw_prev = bw_current;
         bw_current = bw_current -> Next_bw;
       }

       bw_get = (Borrow *)malloc(sizeof(Borrow));

       printf("학번을 입력하세요 : ");
       int nums,numb;
       char yn;
       scanf("%d",&nums);
       getchar();

       c_prev=c_head;
       c_current=c_head->Next_c;
       while(c_current!=c_tail && ((c_current->s_num) < nums)) //name이 작으면 음수, 크면 양수, 같으면 0   name이 ㄱ, arr이 ㅎ이면 음수
       {
           c_prev=c_current;
           c_current=c_current->Next_c;
       }
       if(c_current ->s_num == nums){

         bw_get -> snum = nums;

         printf("\n도서번호를 입력하세요 : ");
         scanf("%d",&numb);
         getchar();
         bw_get -> bnum = numb;

         printf("\n이 도서를 대여합니까? ");
         scanf("%c",&yn);
         getchar();
         if(yn == 'Y'){
           if(bkjoongbok(numb)){
             printf("대여 가능여부 : N\n");
             adminmenu();
           }
           else{
             deyeo();
             bw_get -> Next_bw = bw_tail;
             bw_prev -> Next_bw = bw_get;
             bk_check(bw_get);
             printf("\n도서가 대여 되었습니다.\n");
             bkfile_manage();
             bwfile_manage();
             adminmenu();
           }
         }
         else adminmenu();
       }
       else{
        printf("입력된 학번과 일치하는 회원이 없습니다.\n");
        adminmenu();
      }
}

void dyISBNsearch(void)
{
        unsigned long long ISBN;
        int cnt = 0;
        printf("ISBN을 입력하세요 : ");
        scanf("%13llu",&ISBN);
        getchar();
        bk_prev= bk_head;
        bk_current = bk_prev -> Next_bk;
        printf("\n\n>>검색 결과<<\n");
        while(1){

                if(bk_current == bk_tail)
                        break;

                if(bk_current -> isbn == ISBN){
                  if(joongbok(bk_current)==0){
                        cnumprint();
                        printf("\n도서명 : %s\n",bk_current -> b_name);
                        printf("출판사 : %s\n",bk_current -> publisher);
                        printf("저자명 : %s\n",bk_current -> author);
                        printf("ISBN : %llu\n",bk_current -> isbn);
                        printf("소장처 : %s\n\n\n",bk_current -> location);
                        cnt++;
                      }
                    }
                bk_current = bk_current -> Next_bk;
          }
          if(bk_current == bk_tail){
            if(cnt == 0){
              printf("도서를 찾을 수 없습니다.\n\n\n");
              dyISBNsearch();
            }
          }

              bw_prev = bw_head;
              bw_current = bw_head -> Next_bw;
              while(bw_current != bw_tail){
                bw_prev = bw_current;
                bw_current = bw_current -> Next_bw;
              }

              bw_get = (Borrow *)malloc(sizeof(Borrow));

              printf("학번을 입력하세요 : ");
              int nums, numb;
              char yn;
              scanf("%d",&nums);
              getchar();
              c_prev=c_head;
              c_current=c_head->Next_c;
              while(c_current!=c_tail && ((c_current->s_num) < nums)) //name이 작으면 음수, 크면 양수, 같으면 0   name이 ㄱ, arr이 ㅎ이면 음수
              {
                  c_prev=c_current;
                  c_current=c_current->Next_c;
              }
              if(c_current ->s_num == nums){

                bw_get -> snum = nums;
                printf("\n도서번호를 입력하세요 : ");
                scanf("%d",&numb);
                getchar();
                bw_get -> bnum = numb;

                printf("\n");
                printf("이 도서를 대여합니까? ");
                scanf("%c", &yn);
                getchar();
                if(yn == 'Y'){
                  if(bkjoongbok(numb)){
                    printf("\n대여 가능여부 : N\n");
                    adminmenu();
                  }
                  else{
                    deyeo();
                    bw_get -> Next_bw = bw_tail;
                    bw_prev -> Next_bw = bw_get;
                    bk_check(bw_get);
                    printf("\n도서가 대여 되었습니다.\n");
                    bkfile_manage();
                    bwfile_manage();
                    adminmenu();
                  }
                }
                else adminmenu();
              }
              else{
               printf("\n입력된 학번과 일치하는 학번의 회원이 없습니다.\n");
               adminmenu();
              }
}
int bkjoongbok(int numb){
  bk_prev= bk_head;
  bk_current = bk_prev -> Next_bk;
  while(1){
          if(bk_current == bk_tail)
            break;
          if(bk_current -> b_num == numb){
            if(bk_current -> ox =='N'){
              return 1;
            }
          }
          bk_current = bk_current -> Next_bk;
        }
  return 0;
}

void bk_check(Borrow * bw_get){
  bk_prev=bk_head;
  bk_current=bk_head->Next_bk;
  while(bk_current!=bk_tail) //끝까지 이동
  {
    if(bk_current -> b_num == bw_get -> bnum){
      break;
    }
    bk_prev=bk_current;
    bk_current=bk_current->Next_bk;
  }

  if(bk_current -> b_num == bw_get -> bnum){
    bk_current->ox = 'N';
  }
  else{
    printf("\n입력된 도서번호와 일치하는 도서가 없습니다.\n");
    adminmenu();
  }

}

void deyeo(){
  //여기부터 타임함수 관련
  struct tm *t=NULL,*nt=NULL;
  time_t now,returntime;

  now=time(NULL);

  returntime=time(NULL)+(30*24*60*60);
  nt = localtime(&returntime);
  if(nt->tm_wday==0){
    returntime=time(NULL)+(31*24*60*60);
  }

  bw_get -> borrow_b = now;
  bw_get -> return_b = returntime;
}

void returnbook(){
  int books, cnt = 0;
  char YN;
  printf("\n반납할 도서번호를 입력하세요: ");
  scanf("%d", &books);
  getchar();
  printf("도서 반납처리를 할까요? ");
  scanf("%c", &YN);
  getchar();

  if(YN == 'Y'){
    bk_prev= bk_head;
    bk_current = bk_prev -> Next_bk;

    while(bk_current!=bk_tail)
    {
      if(bk_current->b_num == books){
        cnt++;
        break;
      }
      bk_prev=bk_current;
      bk_current=bk_current->Next_bk;
    }
    if(bk_current == bk_tail){
      if(cnt == 0){
        printf("\n입력된 도서번호와 일치하는 도서가 없습니다.\n\n\n");
        adminmenu();
      }
    }

    bk_current->ox = 'Y';

    cnt = 0;
    bw_prev= bw_head;
    bw_current = bw_prev -> Next_bw;

    while(bw_current!=bw_tail)
    {
      if(bw_current->bnum == books){
        cnt++;
        break;
      }
      bw_prev=bw_current;
      bw_current=bw_current->Next_bw;
    }
    if(bw_current == bw_tail){
      if(cnt == 0){
        printf("\n입력된 도서번호와 일치하는 도서가 없습니다.\n\n\n");
        adminmenu();
      }
    }

    bw_prev -> Next_bw = bw_current -> Next_bw;
    free(bw_current);
    bw_current = bw_prev -> Next_bw;

    printf("\n도서가 반납 되었습니다\n");

    bkfile_manage();
    bwfile_manage();
  }
  else
    printf("\n도서가 반납되지 않았습니다.\n");
  return;
}

void namesearch(void)
{
        char name[100];
        printf("이름을 입력하세요: ");
        scanf("%[^\n]", name);
        printf("\n");

        c_prev= c_head;
        c_current = c_prev -> Next_c;
        printf("\n\n>>검색 결과<<\n");
        while(c_current != c_tail)
        {
          if(c_current == c_tail){
            printf("회원이 없습니다.\n");
            searchclient();
          }
        if(strcmp(c_current->c_name, name) == 0){
          printf("학번: %d\n", c_current -> s_num);
          printf("이름: %s\n", c_current -> c_name);
          printf("주소: %s\n", c_current -> address);
          printf("전화번호: %s\n\n", c_current -> p_num);
        }
        c_prev=c_current;
        c_current=c_current->Next_c;
      }
      c_current=c_tail;
}

void studentidsearch(void)
{
        int id;
        printf("학번을 입력하세요: ");
        scanf("%d",&id);
        getchar();
        printf("\n\n>>검색 결과<<\n");
        c_prev= c_head;
        c_current = c_prev -> Next_c;

        while(c_current!=c_tail && (c_current->s_num < id))
        {
          c_prev=c_current;
          c_current=c_current->Next_c;
        }
        if(c_current->s_num == id){
          printf("학번: %d\n",c_current -> s_num);
          printf("이름: %s\n",c_current -> c_name);
          printf("주소: %s\n", c_current -> address);
          printf("전화번호: %s\n",c_current -> p_num);
        }

        c_current = c_current -> Next_c;

        return;
}

void allclientsearch(void)
{

        c_prev= c_head;
        c_current = c_prev -> Next_c;
        printf("\n\n>>검색 결과<<\n");
        while(c_current != c_tail){
          printf("학번: %d\n",c_current -> s_num);
          printf("이름: %s\n",c_current -> c_name);
          printf("주소: %s\n", c_current -> address);
          printf("전화번호: %s\n\n",c_current -> p_num);

          c_current = c_current -> Next_c;
        }

        return;
}

void cfile_manage(){
  fclose(fp_c);
  fp_c=fopen("client.txt","wt");

  c_current = c_head -> Next_c;
  while(c_current != c_tail)
  {
    fprintf(fp_c,"%d|", c_current->s_num);
    fprintf(fp_c,"%s|", c_current->passwd);
    fprintf(fp_c,"%s|", c_current->c_name);
    fprintf(fp_c,"%s|", c_current->address);
    fprintf(fp_c,"%s\r\n", c_current->p_num);
    c_current=c_current->Next_c;
  }

  fclose(fp_c);
}

void bkfile_manage(){
  fclose(fp_bk);
  fp_bk=fopen("book.txt","wt");

  bk_current = bk_head -> Next_bk;
  while(bk_current != bk_tail)
  {
    fprintf(fp_bk,"%7d|", bk_current->b_num);
    fprintf(fp_bk,"%s|", bk_current->b_name);
    fprintf(fp_bk,"%s|", bk_current->publisher);
    fprintf(fp_bk,"%s|", bk_current->author);
    fprintf(fp_bk,"%13lu|", bk_current->isbn);
    fprintf(fp_bk,"%s|", bk_current->location);
    fprintf(fp_bk,"%c\r\n", bk_current->ox);
    bk_current=bk_current->Next_bk;
  }

  fclose(fp_bk);
}

void bwfile_manage(){
  fclose(fp_bw);
  fp_bw=fopen("borrow.txt","wt");

  bw_current = bw_head -> Next_bw;
  while(bw_current != bw_tail)
  {
    fprintf(fp_bw,"%d|", bw_current->snum);
    fprintf(fp_bw,"%d|", bw_current->bnum);
    fprintf(fp_bw,"%ld|", bw_current->borrow_b);
    fprintf(fp_bw,"%ld\n", bw_current->return_b);
    bw_current=bw_current->Next_bw;
  }

  fclose(fp_bw);
}

void cfree_node(){
  c_prev = c_head;
  c_current = c_head -> Next_c;

  while(c_current != c_tail){
    c_prev -> Next_c = c_current -> Next_c;
      free(c_current);
      c_current = NULL;
      c_current = c_prev -> Next_c;
  }
  free(c_head);
  c_head = NULL;
  free(c_tail);
  c_tail = NULL;
}

void bkfree_node(){
  bk_prev = bk_head;
  bk_current = bk_head -> Next_bk;

  while(bk_current != bk_tail){
    bk_prev -> Next_bk = bk_current -> Next_bk;
      free(bk_current);
      bk_current = NULL;
      bk_current = bk_prev -> Next_bk;
  }
  free(bk_head);
  bk_head = NULL;
  free(bk_tail);
  bk_tail = NULL;
}

void bwfree_node(){
  bw_prev = bw_head;
  bw_current = bw_head -> Next_bw;

  while(bw_current != bw_tail){
    bw_prev -> Next_bw = bw_current -> Next_bw;
      free(bw_current);
      bw_current = NULL;
      bw_current = bw_prev -> Next_bw;
  }
  free(bw_head);
  bw_head = NULL;
  free(bw_tail);
  bw_tail = NULL;
}

int main(void){
  system("clear");
  fp_c = fopen("client.txt","r");
  fp_bk = fopen("book.txt", "r");
  fp_bw = fopen("borrow.txt", "r");
  if(fp_c == NULL||fp_bk==NULL||fp_bw==NULL){
    printf("file error\n");
    exit(1);
  }

  c_head=(Client *)malloc(sizeof(Client));
  c_tail=(Client *)malloc(sizeof(Client));
  c_head->Next_c=c_tail;

  bk_head=(Book *)malloc(sizeof(Book));
	bk_tail=(Book *)malloc(sizeof(Book));
	bk_head->Next_bk=bk_tail;

  bw_head=(Borrow *)malloc(sizeof(Borrow));
	bw_tail=(Borrow *)malloc(sizeof(Borrow));
	bw_head->Next_bw=bw_tail;

  c_read();
  bk_read();
  bw_read();
  mainmenu();
  return 0;
}
