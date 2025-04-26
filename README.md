![image](https://github.com/user-attachments/assets/47cf23c2-6755-4b44-97a9-4b43e0c0705d)# 메모장
> 컴퓨터에서 기본적으로 제공하는 그림판(Paint-Board) 기능을 구현하고 추가 기능을 넣어보기
>
<br />

# 프로젝트 정보
> 1.제작 기간
>>
>
> 2.참여인원
>> 1인 개발
>

<br />

# 사용된 기술
>> C++(MFC)

<br />

# 구현 기능
> 1. 그림판의 기본기능
>> 1. 점
>> 2. 선( 굵기, 색상)
>> 3. 도형(삼각형, 원, 사각형)
>>> 굵기, 내부 색상
>> 4. 자유선
> 2. 더블 버퍼링을 응용한 3중 버퍼링
<details><summary>자세한 내용</summary>
<p>

#### 더블 버퍼링이란?
```
더블 버퍼링(Double Buffering)은 이중 버퍼링이라 불리기도 하며, 그래픽 객체에 이미지를 그릴 때 사용되는 기법이다.
[참고 이미지] ![image](https://github.com/user-attachments/assets/d11bc0cd-ac32-4810-9d95-ab6a8096101a)
사용하는 이유
이미지를 그리는 데 시간이 소요되므로 이미지의 출력이 잦을수록 깜빡거리는 현상이 심해진다.
시각적으로 불편함을 없에기 위해 사용함
```
```
일반적인 더블 버퍼링 = 새로 그리는 도형이 기존 도형과 겹칠 경우 겹치는 부분이 지워지는 현상 발생
따라서 3중 버퍼링을 통해 위의 현상을 해결함
```
</p>
</details>
> 3. 파일저장 및 열기 ( 비트맵(24, 8), VI 파일, RAW, PNG, JPG)
>>
> 4. 도형이 겹쳐있는 경우 뒤에 있는 도형 마우스 우클릭스 뒤에 있는 도형이 앞으로 오는 기능
> 5. 새로 만들기, 저장, 열기
> 6. 명령어 박스


<br />


# 기타 참고했던 자료들
> # 참고 사이트
>> winapi 입문 강좌
>> 
>> http://soen.kr/lecture/win32api/win32lec.htm
>> 
>> http://mafa.tistory.com/entry/API-%EC%9C%88%EB%8F%84%EC%9A%B0-%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%A8%EC%9D%98-%EA%B5%AC%EC%A1%B0
>> 
>> http://klkl0.tistory.com/87
>> 
>> http://wookss-blog.tistory.com/2
>> 
>> http://hackingboy.tistory.com/118
>> 
>> http://hackingboy.tistory.com/117
>> 
>> 메뉴
>> 
>> http://zetcode.com/gui/winapi/menus/
>> 
>> http://soen.kr/lecture/win32api/lec5/lec5-2-2.htm
>> 
>> https://www.daniweb.com/programming/software-development/threads/287455/winapi-getopenfilename-and-getsavefilename-not-working#
>> 
>> 다이얼로그
>> 
>> http://winnerz.tistory.com/entry/Basic-DialogBox%EB%A9%94%EC%9D%B8%EC%B0%BD%EC%9D%84-%EB%8B%A4%EC%9D%B4%EC%96%BC%EB%A1%9C%EA%B7%B8%EB%B0%95%EC%8A%A4%EB%A1%9C-%EB%9D%84%EC%9A%B0%EA%B8%B0
>> 
>> http://blog.naver.com/PostView.nhn?blogId=netrance&logNo=110080318599
>> 
>> VK_가상 키
>> 
>> http://soen.kr/lecture/win32api/lec4/lec4-1-3.htm
>> 
>> https://shaeod.tistory.com/388
<br />
> 기초 내용
>> api는 윈도우즈에 있는 함수집합
>> 커널은 우리가 쓰는 것이 아니고 운영채제가 쓰는거라 건들면 안됨. 예)핸들종류

>> int WINAPI WinMain		//윈도우의 메인문 WINAPI : 윈도우 프로그램이라는 의미
>> 
>> (HINSTANCE hInstance,		//앞에 h가 붙으면 핸들이라고 생각Instance는 사용할 수 있는 객체를 만들어내는 것
>> 				//hInstance: 운영체제의 커널이 응용 프로그램에 부여한 ID
>> 					     운영채제가 어떤 프로그램인지 식별 할 수 있게함
>>  HINSTANCE hPrevInstance,
>> 
>>  LPSTR lpszCmdLine,		//szCmdLine : 커멘트라인 상에서 프로그램 구동 시 전달된 문자열
>> 
>>  int nCmdShow)			//iCmdShow : 윈도우가 화면에 출력될 형태
>> 
>> WNDCLASS WndClass;		//WndClass 라는 구조체 정의(밑에서 .으로 접근) //윈도우 클래스의 구조체 안의  값을 채워서 레지스터에 등록을 한다.
>> 				
>> WndClass.style = CS_HREDRAW | CS_VREDRAW;	//H는 가로, S는 세로-> 창의 가로나 세로의 변화가 생길경우 다시 그린다.
>> 
>> //WndClass.lpfnWndProc = (WNDPROC)WndProc
>>> 기본적으로 윈도우는 메세지 기반 운영체제-> 이벤트(마우스,키보드 등등을 움직이는 것)가 발생했을 때 메세지로 만들어줌
>>> 만들어준 메세지를 큐(선입선출)로 넣는다. 발생 순서대로 처리되야하기 때문
>>> 메세지에 따른 사용자 정의기능을 만들어 낼 수 있어야 한다->메세지 큐에 어떤게 들어가있는지 정확히 알아야함(WndProc가 그런 기능을 함)
>>> WndProc는 밑에있는 함수임. 값을 변경하기 위해서 포인터로 쓴다.
>> -----------------------------------------------------------------------------------------------------------------
>> hwnd = CreateWindow 윈도우 창을 만드는 것
>>

<br />
