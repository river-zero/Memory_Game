## 짝 맞추기 게임
뒤집힌 카드 더미에서 한 장씩 앞면을 확인할 수 있게 뒤집습니다. 짝이 맞으면 없어지고 모든 카드의 짝을 맞추게 되면 클리어하는 방식의 게임입니다. 해당 프로젝트는 GDI+를 이용해 구현하였습니다.

## 플레이 영상
https://github.com/river-zero/Memory_Game/assets/124229424/6352014f-b96c-47ca-9569-8fccd9af334c

## 기능
- 그래픽 요소 표현
  - GameLogic 클래스에서 배경, 카드 더미, 클릭수 그래픽 요소 관리
  - 낱개 카드 이미지는 Card 클래스에서 처리
- 카드 랜덤 생성 및 배치
  - GameLogic 클래스의 CreateCards 함수에서 처리
  - 카드 종류를 균등하게 분배
  - 짝이 맞도록 생성은 두 개씩
  - mt19937과 std::shuffle로 카드를 무작위로 섞어 배치
- 카드 뒤집기
  - Card 클래스에서 처리
  - CheckClicked 함수로 클릭 시 카드의 앞면과 뒷면 상태를 전환
  - InvalidateRect 함수를 호출하여 화면 업데이트 요청
- 짝 맞추기
  - GameLogic 클래스에서 처리
  - 두 개의 카드가 같은 종류라면 화면에서 제거
  - 짝이 맞지 않는 경우 두 카드를 뒤집기
  - 같은 카드를 두 번 클릭한 경우는 해당하지 않음에 주의
- 클릭수 카운팅
  - GameLogic 클래스에서 관리
  - 플레이어가 카드를 클릭할 때마다 mClickCount 멤버 변수가 증가
  - InvalidateRect 함수를 호출해 화면 업데이트 요청

## 시행착오
이미지 크기가 큰 리소스를 사용하였는데 그래픽 요소 표현 시 너비와 높이를 줄여서 그려지도록 하였습니다. 그런데 게임 진행 속도가 현저히 느려지는 문제가 발생했습니다. 무엇이 문제인지 찾다가 이미지 크기를 줄이는 작업이 게임 성능에 영향을 미친다는 것을 알게 되었습니다. 이미지 크기를 애초에 화면에 표시될 때의 크기만큼 줄인 상태에서 프로그래밍하여 CPU 및 GPU 자원 낭비를 줄여 게임 성능을 개선하였습니다.

## 리소스 목록
|   이름    |    설명     |
| :-------: | :---------: |
| Board.jpg |    배경     |
| Back.png  |  카드 뒷면  |
|   7.png   |   7 카드    |
|  Ace.png  | 에이스 카드 |
| Joker.png |  조커 카드  |
| King.png  |   킹 카드   |


## 출처
- https://opengameart.org/content/vintage-playing-cards?page=1
- https://opengameart.org/content/weathered-wood-seamless-texture-with-normalmap
