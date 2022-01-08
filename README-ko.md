# isaac-helper
아이작에서 떨어진 아이템을 확인할 수 있습니다.

## 동기부여
아이템 검색...

## 특징
떨어뜨린 아이템(수집품/장신구/카드/약품)을 집기 전에 사용자에게 보여줍니다.

## 사용된 기술과 종속성
- **기술**
	- Dll 인젝션
	- API & Code 후킹
-  **Dependencies**
	- asmjit
	- asmtk
	- imgui
		- glew
		- glfw
		- stb
	- LeksysINI
	- rapidcsv

## 설치
필수 
- `isaac afterbirth plus`
- [Visual Studio 2022 c++ x86 redistributable package](https://docs.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170)

릴리스에서 바이너리를 다운로드하고 `loader.exe`를 실행합니다.
경우에 따라 관리 권한이 필요할 수 있습니다.

## 지원언어
- 한국어

## How to Use?
게임에서 'H'를 눌러 메뉴를 불러옵니다.
End 키를 눌러 도우미를 종료합니다.

## Build Step

**Requires**
- `Visual Studio 2022` and `vs 구성요소 cmake`
- Python 3.x
---
1. git clone https://github.com/somersby10ml/isaac-helper --recurse-submodules
2.  빌드 asmjit/asmtk/glew/glfw (See the batch file.)
	1.  `x86 Native Tools Command Prompt for VS 2022` 를 실행한 후 deps 폴더로 이동합니다.
	2. 각각의 배치파일을 실행합니다. (자세한 내용은 배치파일 내용참조)
	3. 나온 lib 들은 `deps/lib` 폴더에 있어야합니다.
3. 이미지 추출
	1. `Bin/data` 폴더로 이동
	2. `1.ResourceExtractor.bat` 파일의 `GamePath` 수정후 실행
	3. 파이썬으로  `2.ChangeFileName.py` 파일을 실행합니다.
		( a_001_b.png -> 1.png )
		
## source
[Item list text](https://namu.wiki/w/%EC%95%84%EC%9D%B4%EC%9E%91%EC%9D%98%20%EB%B2%88%EC%A0%9C)

## License
MIT


