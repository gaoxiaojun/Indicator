/*****************************************************************************
 * ���ۿ��ӻ�����ϵͳ
 * Copyright (C) 2016, Martin Tang

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include "Main.h"

//=============================================================================
// ��ѧ��������
//=============================================================================

// ����ɨ�趨λ����
void Parse1(int nCount, float *pOut, float *pHigh, float *pLow)
{
  int nState = -1;
  int nHigh  = 0;
  int nLow   = 0;

  for (int i = 1; i < nCount; i++)
  {
    // �趨Ĭ�����Ϊ��
    pOut[i] = 0;

    // Ѱ�Ҹߵ�ģʽ
    if (nState == 1)
    {
      // �����ǰ��ߴ���֮ǰ��ߣ�����λ����Ϣ
      if (pHigh[i] >= pHigh[nHigh])
      {
        pOut[nHigh] = 0;
        nHigh = i;
        pOut[nHigh] = 1;
      }

      // ȷ��ת��ԭ�ģ���ǰ���С�ڸߵ���ͣ���ǰ���С�ڸߵ���ͣ�
      if ((pHigh[i] < pHigh[nHigh]) && (pLow[i]  < pLow[nHigh]))
      {
        pOut[nHigh] = 1;

        nState = -1;
        nLow   = i;
      }
    }

    // Ѱ�ҵ͵�ģʽ
    else if (nState == -1)
    {
      // �����ǰ���С��֮ǰ��ͣ�����λ����Ϣ
      if (pLow[i] <= pLow[nLow])
      {
        pOut[nLow] = 0;
        nLow = i;
        pOut[nLow] = -1;
      }

      // ȷ��ת��ԭ�ģ���ǰ��ߴ��ڸߵ���ͣ���ǰ��ʹ��ڸߵ���ͣ�
      if ((pLow[i]  > pLow[nLow]) && (pHigh[i] > pHigh[nLow]))
      {
        pOut[nLow] = -1;

        nState = 1;
        nHigh  = i;
      }
    }
  }
}

// ������������5��K�����һ�ʣ�
int Parse2(int nCount, float *pOut, float *pHigh, float *pLow)
{
  int nCurrTop = 0, nPrevTop = 0;
  int nCurrBot = 0, nPrevBot = 0;

  for (int i = 0; i < nCount; i++)
  {
    // �����ߵ㣬�ϲ����������Σ������ϣ�
    if (pOut[i] == 1)
    {
      // ����λ����Ϣ
      nPrevTop = nCurrTop;
      nCurrTop = i;

      // ���Ϸ��ԣ��ϸ�����������γ�һ�ʣ�
      if (((nCurrTop - nCurrBot < 4) && (nCount   - nCurrTop > 4)) ||
           (nCurrBot - nPrevTop < 4) || (nPrevTop - nPrevBot < 4))
      {
        // ����С��������߶Σ�ȥ���м�һ��
        if ((pHigh[nCurrTop] >= pHigh[nPrevTop]) &&
            (pLow [nCurrBot] >  pLow [nPrevBot]))
        {
          pOut[nCurrBot] = 0;
          pOut[nPrevTop] = 0;
        }
      }
      else
      {
      }
    }

    // �����͵㣬�ϲ������½��Σ������£�
    if (pOut[i] == -1)
    {
      // ����λ����Ϣ
      nPrevBot = nCurrBot;
      nCurrBot = i;

      // ���Ϸ��ԣ��ϸ�����������γ�һ�ʣ�
      if (((nCurrBot - nCurrTop < 4) && (nCount   - nCurrBot > 4)) ||
           (nCurrTop - nPrevBot < 4) || (nPrevBot - nPrevTop < 4))
      {
        // ����С��������߶Σ�ȥ���м�һ��
        if ((pLow [nCurrBot] <= pLow [nPrevBot]) &&
            (pHigh[nCurrTop] <  pHigh[nPrevTop]))
        {
          pOut[nCurrTop] = 0;
          pOut[nPrevBot] = 0;
        }
      }
      else
      {
      }
    }
  }
}

//=============================================================================
// �������1�ţ��߶θߵ͵����ź�
//=============================================================================

void Func1(int nCount, float *pOut, float *pHigh, float *pLow, float *pTime)
{
  // ��Ѱ���еĸߵ͵�
  Parse1(nCount, pOut, pHigh, pLow);

  // �������õı��������л��򣨵ڹ��㷨��
  for (int i = 0; i < *pTime; i++)
  {
    Parse2(nCount, pOut, pHigh, pLow);
  }
}

//=============================================================================
// �������2�ţ�����ߵ�����
//=============================================================================

void Func2(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
  CCentroid Centroid;

  for (int i = 0; i < nCount; i++)
  {
    if (pIn[i] == 1)
    {
      // �����߶θߵ㣬���������㷨
      if (Centroid.PushHigh(i, pHigh[i]))
      {
        // �����ڸ�����õ����������
        for (int j = Centroid.nStart; j <= Centroid.nEnd; j++)
        {
          pOut[j] = Centroid.fPHigh;
        }
      }
    }
    else if (pIn[i] == -1)
    {
      // �����߶ε͵㣬���������㷨
      if (Centroid.PushLow(i, pLow[i]))
      {
        // �����ڸ�����õ����������
        for (int j = Centroid.nStart; j <= Centroid.nEnd; j++)
        {
          pOut[j] = Centroid.fPHigh;
        }
      }
    }

    // β��δ������ദ��
    if (Centroid.bValid && (Centroid.nLines >= 2) && (i == nCount - 1))
    {
      for (int j = Centroid.nStart; j < nCount; j++)
      {
        pOut[j] = Centroid.fHigh;
      }
    }
  }
}

//=============================================================================
// �������3�ţ�����͵�����
//=============================================================================

void Func3(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
  CCentroid Centroid;

  for (int i = 0; i < nCount; i++)
  {
    if (pIn[i] == 1)
    {
      // �����߶θߵ㣬���������㷨
      if (Centroid.PushHigh(i, pHigh[i]))
      {
        // �����ڸ�����õ����������
        for (int j = Centroid.nStart; j <= Centroid.nEnd; j++)
        {
          pOut[j] = Centroid.fPLow;
        }
      }
    }
    else if (pIn[i] == -1)
    {
      // �����߶ε͵㣬���������㷨
      if (Centroid.PushLow(i, pLow[i]))
      {
        // �����ڸ�����õ����������
        for (int j = Centroid.nStart; j <= Centroid.nEnd; j++)
        {
          pOut[j] = Centroid.fPLow;
        }
      }
    }

    // β��δ������ദ��
    if (Centroid.bValid && (Centroid.nLines >= 2) && (i == nCount - 1))
    {
      for (int j = Centroid.nStart; j < nCount; j++)
      {
        pOut[j] = Centroid.fLow;
      }
    }
  }
}

//=============================================================================
// �������4�ţ�������㡢�յ��ź�
//=============================================================================

void Func4(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
  CCentroid Centroid;

  for (int i = 0; i < nCount; i++)
  {
    if (pIn[i] == 1)
    {
      // �����߶θߵ㣬���������㷨
      if (Centroid.PushHigh(i, pHigh[i]))
      {
        // ���б��
        pOut[Centroid.nStart] = 1;
        pOut[Centroid.nEnd]   = 2;
      }
    }
    else if (pIn[i] == -1)
    {
      // �����߶ε͵㣬���������㷨
      if (Centroid.PushLow(i, pLow[i]))
      {
        // ���б��
        pOut[Centroid.nStart] = 1;
        pOut[Centroid.nEnd]   = 2;
      }
    }

    // β��δ������ദ��
    if (Centroid.bValid && (Centroid.nLines >= 2) && (i == nCount - 1))
    {
      pOut[Centroid.nStart] = 1;
      pOut[nCount-1]        = 2;
    }
  }
}

//=============================================================================
// �������5�ţ���ͼ�������ź�
//=============================================================================

void Func5(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
  CCentroid Centroid;

  for (int i = 0; i < nCount; i++)
  {
    if (pIn[i] == 1)
    {
      if (Centroid.PushHigh(i, pHigh[i]))
      {
        // �����������ź�
        pOut[i] = 13;
      }
      else if (Centroid.fTop1 < Centroid.fTop2)
      {
        // �ڶ��������ź�
        pOut[i] = 12;
      }
      else
      {
        pOut[i] = 0;
      }
    }
    else if (pIn[i] == -1)
    {
      if (Centroid.PushLow(i, pLow[i]))
      {
        // ����������ź�
        pOut[i] = 3;
      }
      else if (Centroid.fBot1 > Centroid.fBot2)
      {
        // �ڶ�������ź�
        pOut[i] = 2;
      }
      else
      {
        pOut[i] = 0;
      }
    }
  }
}

//=============================================================================
// �������6�ţ�ѡ�����������ź�
//=============================================================================

void Func6(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
  CCentroid Centroid;

  for (int i = 0; i < nCount; i++)
  {
    // �����߶θߵ�
    if (pIn[i] == 1)
    {
      // ���������㷨����
      if (Centroid.PushHigh(i, pHigh[i]) && (Centroid.fBot1 < Centroid.fBot2))
      {
        // ����������
        pOut[i] = 13;
        pOut[nCount-1] = 13;
      }
      else if ((Centroid.fTop1 < Centroid.fTop2) &&
               (Centroid.fBot1 < Centroid.fBot2) &&
               (Centroid.fTop1 > Centroid.fBot2))
      {
        // �ڶ�������
        pOut[i] = 12;
        pOut[nCount-1] = 12;
      }
      else
      {
        pOut[i] = 0;
        pOut[nCount-1] = 0;
      }
    }

    // �����߶ε͵�
    else if (pIn[i] == -1)
    {
      // ���������㷨����
      if (Centroid.PushLow(i, pLow[i]) && (Centroid.fTop1 > Centroid.fTop2))
      {
        // ���������
        pOut[i] = 3;
        pOut[nCount-1] = 3;
      }
      else if ((Centroid.fBot1 > Centroid.fBot2) &&
               (Centroid.fTop1 > Centroid.fTop2) &&
               (Centroid.fBot1 < Centroid.fTop2))
      {
        // �ڶ������
        pOut[i] = 2;
        pOut[nCount-1] = 2;
      }
      else
      {
        pOut[i] = 0;
        pOut[nCount-1] = 0;
      }
    }
  }
}

//=============================================================================
// �������7�ţ��߶�ǿ�ȷ���ָ��
//=============================================================================

void Func7(int nCount, float *pOut, float *pIn, float *pHigh, float *pLow)
{
  int nStatus = 0, nPrevTop, nPrevBot;
  int nRiseNum, nFallNum;

  for (int i = 0; i < nCount; i++)
  {
    // �����߶θߵ�
    if (pIn[i-1] == 1)
    {
      // ��Ǹߵ�λ��
      nStatus  = -1;
      nPrevTop = i - 1;
      nFallNum = 0;
    }
    // �����߶ε͵�
    else if (pIn[i-1] == -1)
    {
      // ��ǵ͵�λ��
      nStatus  = 1;
      nPrevBot = i - 1;
      nRiseNum = 0;
    }

    // �����߶μ���ģʽ
    if (nStatus == 1)
    {
      // ���������߶�б��
      pOut[i] = (pHigh[i] - pLow[nPrevBot]) / (i - nPrevBot)
        / pLow[nPrevBot] * 100;
    }
    // �½��߶μ���ģʽ
    else if (nStatus == -1)
    {
      // ���������߶�б��
      pOut[i] = (pLow[i] - pHigh[nPrevTop]) / (i - nPrevTop)
        / pHigh[nPrevTop] * 100;
    }
  }
}

static PluginTCalcFuncInfo Info[] =
{
  {1, &Func1},
  {2, &Func2},
  {3, &Func3},
  {4, &Func4},
  {5, &Func5},
  {6, &Func6},
  {7, &Func7},
  {0, NULL},
};

BOOL RegisterTdxFunc(PluginTCalcFuncInfo **pInfo)
{
  if (*pInfo == NULL)
  {
    *pInfo = Info;

    return TRUE;
  }

  return FALSE;
}
