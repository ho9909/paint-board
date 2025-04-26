# 그림판
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
>> <details><summary>자세한 내용</summary>
>> <p>
>> 
>> #### 더블 버퍼링이란?
>> ```
>> 더블 버퍼링(Double Buffering)은 이중 버퍼링이라 불리기도 하며, 그래픽 객체에 이미지를 그릴 때 사용되는 기법이다.
>> 사용하는 이유
>> 이미지를 그리는 데 시간이 소요되므로 이미지의 출력이 잦을수록 깜빡거리는 현상이 심해진다.
>> 시각적으로 불편함을 없에기 위해 사용함
>> ```
>> ```
>> 일반적인 더블 버퍼링 = 새로 그리는 도형이 기존 도형과 겹칠 경우 겹치는 부분이 지워지는 현상 발생
>> 따라서 3중 버퍼링을 통해 위의 현상을 해결함
>> ```
>> </p>
>> </details>
> 3. 파일저장 및 열기 ( 비트맵(24, 8), VI 파일, RAW, PNG, JPG)
>> <details><summary>VI 파일 설명</summary>
>> <p>
>> #### VI파일 이란?
>> <br />
>> <img src="https://github.com/user-attachments/assets/1c410716-862d-41cf-bd78-b2917913c6f1" width="50%">
>> 
>>
>> </p>
>> </details>
> 4. 도형이 겹쳐있는 경우 뒤에 있는 도형 마우스 우클릭스 뒤에 있는 도형이 앞으로 오는 기능
> 5. 새로 만들기, 저장, 열기
> 6. 명령어 박스


<br />


# 기타 참고했던 자료들
<br />
> 참고 이미지(더블버퍼링)
<br />
<br />
<img src="https://github.com/user-attachments/assets/33d4ce02-75b9-430f-9519-e73de39f55fc" width="50%" />
<br />
<br />
<img src="https://github.com/user-attachments/assets/1d298c8d-f40e-47cb-a5ef-61e6e2e04857" width="50%" />
<br />
<br />
> 비트맵
<br />
https://github.com/ho9909/paint-board/blob/main/%EB%B9%84%ED%8A%B8%EB%A7%B5.pdf

