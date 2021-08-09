//----------------------------------------------------------------------------

//���������� ���������� ������ TDac

//----------------------------------------------------------------------------

#ifndef DAC_HPP
#define DAC_HPP

//----------------------------- ���������: -----------------------------------

enum dacs_t { DAC1, DAC2 }; //���� ������� DAC
#define DAC_RES      12 //DAC resolution, bits
#define DAC_MAX_CODE ((1 << DAC_RES) - 1) //DAC max code
#define DAC_BOFF      0 //DAC buffer off

//----------------------------------------------------------------------------
//-------------------------- ��������� ����� TDac: ---------------------------
//----------------------------------------------------------------------------

template<dacs_t DacN>
class TDac
{
private:
  TGpio<PORTA, (DacN == DAC1)? PIN4 : PIN5> Pin_DAC;
public:
  TDac(void) {};
  void Init(void);
  void Off(void);
  void operator = (uint16_t Value);
};

//---------------------------- �������������: --------------------------------

template<dacs_t DacN>
void TDac<DacN>::Init(void)
{
#ifdef DAC_AVAIL
  Pin_DAC.Init(IN_ANALOG);            //��������� ����� DAC
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;  //��������� ������������ DAC

  if(DacN == DAC1) //��������� ��������, ����������� ������ ��������
  {
    DAC->CR |=
      DAC_CR_DMAEN1    * 0 | //DMA disable
      DAC_CR_MAMP1_0   * 0 | //mask/amplitude select
      DAC_CR_WAVE1_0   * 0 | //wave generation disabled
      DAC_CR_TSEL1_0   * 0 | //trigger select
      DAC_CR_TEN1      * 0 | //trigger disable
      DAC_CR_BOFF1     * DAC_BOFF | //buffer on/off
      DAC_CR_EN1       * 1;  //DAC1 enable
  }
  if(DacN == DAC2) //��������� ��������, ����������� ������ ��������
  {
    DAC->CR |=
      DAC_CR_DMAEN2    * 0 | //DMA disable
      DAC_CR_MAMP2_0   * 0 | //mask/amplitude select
      DAC_CR_WAVE2_0   * 0 | //wave generation disabled
      DAC_CR_TSEL2_0   * 0 | //trigger select
      DAC_CR_TEN2      * 0 | //trigger disable
      DAC_CR_BOFF2     * DAC_BOFF | //buffer on/off
      DAC_CR_EN2       * 1;  //DAC2 enable
  }
#endif  
}

//----------------------------- ����������: ----------------------------------

template<dacs_t DacN>
void TDac<DacN>::Off(void)
{
#ifdef DAC_AVAIL
  Pin_DAC.Init(IN_ANALOG);   //��������� ����� DAC
  if(DacN == DAC1) //��������� ��������, ����������� ������ ��������
  {
    DAC->CR &= ~DAC_CR_EN1;  //DAC1 disable
    if((DAC->CR & DAC_CR_EN2) == 0) RCC->APB1ENR &= ~RCC_APB1ENR_DACEN;
  }
  if(DacN == DAC2) //��������� ��������, ����������� ������ ��������
  {
    DAC->CR &= ~DAC_CR_EN2;  //DAC2 disable
    if((DAC->CR & DAC_CR_EN1) == 0) RCC->APB1ENR &= ~RCC_APB1ENR_DACEN;
  }
#endif 
}

//------------------------------ ��������: -----------------------------------

template<dacs_t DacN>
void TDac<DacN>::operator = (uint16_t Value)
{
#ifdef DAC_AVAIL
  //����������� ���������:
  if(Value > DAC_MAX_CODE) Value = DAC_MAX_CODE;
  //��������:
  if(DacN == DAC1)
  {
    DAC->DHR12R1 = Value;
  }
  if(DacN == DAC2)
  {
    DAC->DHR12R2 = Value;
  }
#endif 
}

//----------------------------------------------------------------------------

#endif
