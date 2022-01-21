uint8_t Buf1[12144][3];
uint8_t Buf2[12144][3];

void setup() {

  pinMode(PD_0, OUTPUT); // out 1
  pinMode(PD_1, OUTPUT); // out 2
  pinMode(PD_2, OUTPUT); // out 3
  pinMode(PD_3, OUTPUT); // out 4
  pinMode(PD_4, OUTPUT); // out 5
  pinMode(PD_5, OUTPUT); // out 6
  pinMode(PD_6, OUTPUT); // out 7
  pinMode(PD_7, OUTPUT); // out 8


  RCC->AHB1ENR |= 1 << 22; // dma2 clock enable
  RCC->APB2ENR |= 1;// tim1 clock enable
  TIM1->DIER = (1 << 8); //dma tick > Update timer
  TIM1->PSC = 34; // 35 - 1 !!       tim devider..
  TIM1->ARR = 1; // dev2
  TIM1->CR1 = 1; //timer start
  DMA2_Stream5->CR = (6 << 25) | (0 << 11) | (1 << 10) | (1 << 8) | (1 << 6) | (1 << 18); //Ch:6,WORD,MINC,CIRC,Mem2per,double buffer mode
  DMA2_Stream5->M0AR = (uint32_t) Buf1; //dma memory buffer 1 sourse
  DMA2_Stream5->M1AR = (uint32_t) Buf2;
  DMA2_Stream5->PAR = (uint32_t) & (GPIOD->ODR); //dma per. dest.
  DMA2_Stream5->NDTR = 36432; DMA2_Stream5->CR |= 1; //start DMA

 
  for (uint16_t i = 0; i < 12144; i++) //pix 0-505 struct
  {
    Buf1[i][0] = 0xff;
    Buf1[i][2] = 0;
  }
  for (uint16_t i = 0; i < 12048; i++) //pix 506-1008 struct + reset time
  {
    Buf2[i][0] = 0xff;
    Buf2[i][2] = 0;
  }
}

void setpix(uint8_t out, uint16_t led, uint8_t r, uint8_t g, uint8_t b)
{
  if (led < 506)
  {
    for (uint8_t bt = 0; bt < 8; bt++)
    {
      bitWrite(Buf1[bt + led * 24][1], out, bitRead(g, (7 - bt)));
      bitWrite(Buf1[bt + led * 24 + 8][1], out, bitRead(r, (7 - bt)));
      bitWrite(Buf1[bt + led * 24 + 16][1], out, bitRead(b, (7 - bt)));
    }
  }
  else if (led >= 506 && led < 1008)
  {
    for (uint8_t bt = 0; bt < 8; bt++)
    {
      bitWrite(Buf2[bt + (led - 506) * 24][1], out, bitRead(g, (7 - bt)));
      bitWrite(Buf2[bt + (led - 506) * 24 + 8][1], out, bitRead(r, (7 - bt)));
      bitWrite(Buf2[bt + (led - 506) * 24 + 16][1], out, bitRead(b, (7 - bt)));
    }
  }
}

void loop() 
{

  for (uint16_t i = 0; i < 1008; i++) // 10% red to all pix for out 1-4 
  {
    setpix(0, i, 25, 0, 0); 
    setpix(1, i, 25, 0, 0);
    setpix(2, i, 25, 0, 0);
    setpix(3, i, 25, 0, 0);
  }

  delay(1000);
  
  for (uint16_t i = 0; i < 1008; i++) // black to all pix for out 1-4 
  {
    setpix(0, i, 0, 0, 0);
    setpix(1, i, 0, 0, 0);
    setpix(2, i, 0, 0, 0);
    setpix(3, i, 0, 0, 0);
  }
  delay(1000);
}
