

//2类数据(日冻结,月冻结,曲线)各Fn最大Pn
#ifndef AFN0DMaxPn_H
#define AFN0DMaxPn_H


#if (Project/100)==0//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#include "AFN0DMaxPn_Project0xx.h"
#endif
#if (Project/100)==1//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#include "AFN0DMaxPn_Project1xx.h"
#endif
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#include "AFN0DMaxPn_Project2xx.h"
#endif
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#include "AFN0DMaxPn_Project3xx.h"
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#include "AFN0DMaxPn_Project4xx.h"
#endif
#if (Project/100)==5//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#include "AFN0DMaxPn_Project5xx.h"
#endif
#if (Project/100)==6//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#include "AFN0DMaxPn_Project6xx.h"
#endif


#endif
