/*
	이 프로그램은 안릉란STM32-X4, STM32-V5 개발보드에 적합합니다.

	다른 하드웨어에 사용할 경우, GPIO 정의와 IsKeyDown1 - IsKeyDown8 함수를 수정하세요.

	사용자가 8개 미만의 버튼을 가진 경우, 나머지 버튼을 첫 번째 버튼과 동일하게 정의하면 프로그램 기능에 영향이 없습니다.
	#define KEY_COUNT    8	  이것은 bsp_key.h 파일에 정의되어 있습니다.
*/
#include "bsp.h"


#ifdef STM32_X3		/* 안릉란 STM32-X4 개발보드 */
	/*
		안릉란STM32-X 버튼 포트 할당:
			K1 키       : PC13 (낮은 레벨은 누름 상태)
			K2 키       : PC0  (낮은 레벨은 누름 상태)
			K3 키       : PC1  (낮은 레벨은 누름 상태)
			조이스틱UP  : PC2  (낮은 레벨은 누름 상태)
			조이스틱DOWN: PC3  (낮은 레벨은 누름 상태)
			조이스틱LEFT: PC4  (낮은 레벨은 누름 상태)
			조이스틱RIGHT: PA0  (낮은 레벨은 누름 상태)
			조이스틱OK  : PA1  (낮은 레벨은 누름 상태)
	*/
	#define RCC_ALL_KEY 	(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC)	/* 버튼 포트 RCC 클럭 */

	#define GPIO_PORT_K1    GPIOC
	#define GPIO_PIN_K1	    GPIO_Pin_13

	#define GPIO_PORT_K2    GPIOC
	#define GPIO_PIN_K2	    GPIO_Pin_0

	#define GPIO_PORT_K3    GPIOC
	#define GPIO_PIN_K3	    GPIO_Pin_1

	#define GPIO_PORT_K4    GPIOC
	#define GPIO_PIN_K4	    GPIO_Pin_3

	#define GPIO_PORT_K5    GPIOC
	#define GPIO_PIN_K5	    GPIO_Pin_4

	#define GPIO_PORT_K6    GPIOC
	#define GPIO_PIN_K6	    GPIO_Pin_2

	#define GPIO_PORT_K7    GPIOA
	#define GPIO_PIN_K7	    GPIO_Pin_1

	#define GPIO_PORT_K8    GPIOA
	#define GPIO_PIN_K8	    GPIO_Pin_0

#else	/* STM32_V5 */
	/*
		안릉란STM32-V5 버튼 포트 할당:
			K1 키      : PI8   (낮은 레벨은 누름 상태)
			K2 키      : PC13  (낮은 레벨은 누름 상태)
			K3 키      : PI11  (낮은 레벨은 누름 상태)
			조이스틱UP  : PH2   (낮은 레벨은 누름 상태)
			조이스틱DOWN: PH3   (낮은 레벨은 누름 상태)
			조이스틱LEFT: PF11  (낮은 레벨은 누름 상태)
			조이스틱RIGHT: PG7   (낮은 레벨은 누름 상태)
			조이스틱OK  : PH15  (낮은 레벨은 누름 상태)
	*/

	/* 버튼 포트 RCC 클럭 */
	#define RCC_ALL_KEY 	(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOG)

	#define GPIO_PORT_K1    GPIOI
	#define GPIO_PIN_K1	    GPIO_PIN_8

	#define GPIO_PORT_K2    GPIOC
	#define GPIO_PIN_K2	    GPIO_PIN_13

	#define GPIO_PORT_K3    GPIOI
	#define GPIO_PIN_K3	    GPIO_PIN_11

	#define GPIO_PORT_K4    GPIOH
	#define GPIO_PIN_K4	    GPIO_PIN_2

	#define GPIO_PORT_K5    GPIOH
	#define GPIO_PIN_K5	    GPIO_PIN_3

	#define GPIO_PORT_K6    GPIOF
	#define GPIO_PIN_K6	    GPIO_PIN_11

	#define GPIO_PORT_K7    GPIOG
	#define GPIO_PIN_K7	    GPIO_PIN_7

	#define GPIO_PORT_K8    GPIOH
	#define GPIO_PIN_K8	    GPIO_PIN_15
#endif

static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;		/* 키 FIFO 변수,구조체 */

static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);
//static void bsp_PutKey(uint8_t _KeyCode);

/*
*********************************************************************************************************
*	함수 이름: IsKeyDownX
*	기능 설명: 버튼이 눌렸는지 판단
*	매개 변수: 없음
*	반환 값: 1 - 눌림, 0 - 눌리지 않음
*********************************************************************************************************
*/
#ifdef STM32_X3		/* 안릉란 STM32-X3 개발보드 */
	static uint8_t IsKeyDown1(void) {if ((GPIO_PORT_K1->IDR & GPIO_PIN_K1) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown2(void) {if ((GPIO_PORT_K2->IDR & GPIO_PIN_K2) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown3(void) {if ((GPIO_PORT_K3->IDR & GPIO_PIN_K3) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown4(void) {if ((GPIO_PORT_K4->IDR & GPIO_PIN_K4) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown5(void) {if ((GPIO_PORT_K5->IDR & GPIO_PIN_K5) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown6(void) {if ((GPIO_PORT_K6->IDR & GPIO_PIN_K6) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown7(void) {if ((GPIO_PORT_K7->IDR & GPIO_PIN_K7) == 0) return 1;else return 0;}
	static uint8_t IsKeyDown8(void) {if ((GPIO_PORT_K8->IDR & GPIO_PIN_K8) == 0) return 1;else return 0;}
#else				/* 안릉란 STM32-V5 개발보드 */
	static uint8_t IsKeyDown1(void){return (HAL_GPIO_ReadPin(GPIO_PORT_K1, GPIO_PIN_K1) == GPIO_PIN_RESET) ? 1 : 0;}
	static uint8_t IsKeyDown2(void) {return (HAL_GPIO_ReadPin(GPIO_PORT_K2, GPIO_PIN_K2) == GPIO_PIN_RESET) ? 1 : 0;}
	static uint8_t IsKeyDown3(void){return (HAL_GPIO_ReadPin(GPIO_PORT_K3, GPIO_PIN_K3) == GPIO_PIN_RESET) ? 1 : 0;}
	static uint8_t IsKeyDown4(void) {return (HAL_GPIO_ReadPin(GPIO_PORT_K4, GPIO_PIN_K4) == GPIO_PIN_RESET) ? 1 : 0;}
	static uint8_t IsKeyDown5(void) {return (HAL_GPIO_ReadPin(GPIO_PORT_K5, GPIO_PIN_K5) == GPIO_PIN_RESET) ? 1 : 0;}
	static uint8_t IsKeyDown6(void) {return (HAL_GPIO_ReadPin(GPIO_PORT_K6, GPIO_PIN_K6) == GPIO_PIN_RESET) ? 1 : 0;}
	static uint8_t IsKeyDown7(void) {return (HAL_GPIO_ReadPin(GPIO_PORT_K7, GPIO_PIN_K7) == GPIO_PIN_RESET) ? 1 : 0;}
	static uint8_t IsKeyDown8(void) {return (HAL_GPIO_ReadPin(GPIO_PORT_K8, GPIO_PIN_K8) == GPIO_PIN_RESET) ? 1 : 0;}
#endif
	static uint8_t IsKeyDown9(void) {if (IsKeyDown1() && IsKeyDown2()) return 1;else return 0;}
	static uint8_t IsKeyDown10(void) {if (IsKeyDown1() && IsKeyDown2()) return 1;else return 0;}



/*
*********************************************************************************************************
* 함수 이름: bsp_InitKey
*********************************************************************************************************
*/
void bsp_InitKey(void)
{
	bsp_InitKeyVar();
	bsp_InitKeyHard();
}

/*
*********************************************************************************************************
* 함수 이름: bsp_InitKeyHard
*********************************************************************************************************
*/
static void bsp_InitKeyHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// #define RCC_ALL_KEY 	(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOG)
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();


	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP;  // 풀업 저항 사용 (버튼이 GND에 연결되어 있을 경우)
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStructure.Pin = GPIO_PIN_K1;
	HAL_GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K2;
	HAL_GPIO_Init(GPIO_PORT_K2, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K3;
	HAL_GPIO_Init(GPIO_PORT_K3, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K4;
	HAL_GPIO_Init(GPIO_PORT_K4, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K5;
	HAL_GPIO_Init(GPIO_PORT_K5, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K6;
	HAL_GPIO_Init(GPIO_PORT_K6, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K7;
	HAL_GPIO_Init(GPIO_PORT_K7, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_K8;
	HAL_GPIO_Init(GPIO_PORT_K8, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	함수 이름: bsp_InitKeyVar
*********************************************************************************************************
*/
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	/* 키 FIFO 읽기/쓰기 포인터 초기화 */
	s_tKey.Read = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	/* 각 버튼 구조체 멤버 변수 초기화 */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			/* 장시간 누름 시간. 0이면 장시간 누름 이벤트 없음 */
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* 카운터를 필터링 시간의 절반으로 설정 */
		s_tBtn[i].State = 0;							/* 버튼 상태, 0은 누르지 않음 */
		//s_tBtn[i].KeyCodeDown = 3 * i + 1;				/* 버튼 눌림 키코드 */
		//s_tBtn[i].KeyCodeUp = 3 * i + 2;				/* 버튼 뗌 키코드 */
		//s_tBtn[i].KeyCodeLong = 3 * i + 3;				/* 버튼 장시간 누름 키코드 */
		s_tBtn[i].RepeatSpeed = 0;						/* 연속 입력 속도, 0은 연속 입력 없음 */
		s_tBtn[i].RepeatCount = 0;						/* 연속 입력 카운터 */
	}

	/* 특정 버튼의 파라미터를 개별적으로 변경할 경우 */
	/* 예를 들어, 버튼 1을 1초 이상 누르면 자동으로 동일한 키코드 발송 */
	s_tBtn[KID_JOY_U].LongTime = 100;
	s_tBtn[KID_JOY_U].RepeatSpeed = 5;	/* 50ms마다 자동으로 키코드 발송 */

	s_tBtn[KID_JOY_D].LongTime = 100;
	s_tBtn[KID_JOY_D].RepeatSpeed = 5;

	s_tBtn[KID_JOY_L].LongTime = 100;
	s_tBtn[KID_JOY_L].RepeatSpeed = 5;

	s_tBtn[KID_JOY_R].LongTime = 100;
	s_tBtn[KID_JOY_R].RepeatSpeed = 5;

	/* 버튼 눌림 감지 함수 설정 */
	s_tBtn[0].IsKeyDownFunc = IsKeyDown1;
	s_tBtn[1].IsKeyDownFunc = IsKeyDown2;
	s_tBtn[2].IsKeyDownFunc = IsKeyDown3;
	s_tBtn[3].IsKeyDownFunc = IsKeyDown4;
	s_tBtn[4].IsKeyDownFunc = IsKeyDown5;
	s_tBtn[5].IsKeyDownFunc = IsKeyDown6;
	s_tBtn[6].IsKeyDownFunc = IsKeyDown7;
	s_tBtn[7].IsKeyDownFunc = IsKeyDown8;

	/* 조합 키 */
	s_tBtn[8].IsKeyDownFunc = IsKeyDown9;
	s_tBtn[9].IsKeyDownFunc = IsKeyDown10;
}

/*
*********************************************************************************************************
*	함수 이름: bsp_PutKey
*	기능 설명: 키 이벤트를 FIFO에 넣음
*	매개 변수: _KeyCode - 키 코드
*	반환 값: 없음
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}

/*
*********************************************************************************************************
*	함수 이름: bsp_DetectKey
*	기능 설명: 키 상태를 감지하고 이벤트를 FIFO에 넣음
*	매개 변수: i - 키 인덱스
*	반환 값: 없음
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn;

	pBtn = &s_tBtn[i];

	if (pBtn->IsKeyDownFunc())
	{
		if (pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if (pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1;
				/* 키 누름 이벤트 발생, FIFO에 추가 */
				bsp_PutKey((uint8_t)(3 * i + 1));
			}

			if (pBtn->LongTime > 0)
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						/* 키 장시간 누름 이벤트 발생, FIFO에 추가 */
						bsp_PutKey((uint8_t)(3 * i + 3));
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0)
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* 키 반복 이벤트 발생, FIFO에 추가 */
							bsp_PutKey((uint8_t)(3 * i + 1));
						}
					}
				}
			}
		}
	}
	else
	{
		if (pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if (pBtn->Count > 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;
				/* 키 뗌 이벤트 발생, FIFO에 추가 */
				bsp_PutKey((uint8_t)(3 * i + 2));
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	함수 이름: bsp_KeyScan
*	기능 설명: 모든 키를 스캔하고 이벤트를 처리
*	매개 변수: 없음
*	반환 값: 없음
*********************************************************************************************************
*/
void bsp_KeyScan(void)
{
	uint8_t i;

	for (i = 0; i < KEY_COUNT; i++)
	{
		bsp_DetectKey(i);
	}
}

/*
*********************************************************************************************************
*	함수 이름: bsp_GetKey2
*********************************************************************************************************
*/
uint8_t bsp_GetKey2(void)
{
	uint8_t ret;

	if (s_tKey.Read2 == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read2];

		if (++s_tKey.Read2 >= KEY_FIFO_SIZE)
		{
			s_tKey.Read2 = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	함수 이름: bsp_GetKey
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}







































