# YAK
YAK 

스마트약통 

코드상 해야할거
1. 모터 각도, 시간 조절
2. 어플 연동
3. LCD 디스플레이
4. 무게센서 정확하게 하기
5. 등등

i2c 통신 때문에 PD0 PD1 못 씀
그래서 인터럽트0번 1번 다른곳으로 옮겨야 함

원래있던 PD0 PD1에 꽂힌 것을 PE6, 7에 옮겨야 할 듯
(INT0,1 -> INT6,7)
