/*
** FT2PLAY v0.90 - 21st of June 2018 - https://16-bits.org
** =======================================================
**               - NOT BIG ENDIAN SAFE! -
**
** Very accurate C port of Fasttracker 2.09a's replayer,
** by Olav "8bitbubsy" Sørensen. Using the original pascal+asm source
** codes by Fredrik "Mr.H" Huss (Triton). I have permission to make this
** direct C port public.
**
** Thanks to aciddose for coding the volume ramping.
** I have modified it to be close to FT2-exact.
** NOTE: The audio channel mixer in this replayer is very unoptimized!
**
** (extreme) non-FT2 extensions:
** - Max 127 channels (was 32)
** - Non-even amount-of-channels number (FT2 supports *even* numbers only)
** - Max 256 instruments (was 128)
** - Max 32 samples per instrument (was 16)
** - Max 1024 rows per pattern (was 256)
** - Stereo samples
**
** These additions shouldn't break FT2 accuracy unless I'm wrong.
**
** You need to link winmm.lib for this to compile (-lwinmm)
** Alternatively, you can wrap around your own audio callback for using
** ft2play on other platforms (read comment near bottom).
**
**
** Changes from 0.88:
** - Forgot to free a block of memory on close
**
** Changes from 0.88:
** - Fixed a buffer overflow bug in patternEmpty()
** - Smaller memory footprint for pattern data
** - Tiny code cleanup
**
** Changes from 0.87:
** - Fixed an off-by-one error in startTone() that clamp notes wrongly
**
** Changes from 0.86:
** - Separate WinMM mixer code from replayer, making it easier to use for other
**
** Changes from 0.85:
** - Fixed a bug where triggering empty samples wouldn't kill the voice
**
** Version 0.85:
** - Rewritten to be closer to the replayer used in the FT2 clone
**
**
** ft2play.h:
**
** #include <stdint.h>
**
** int8_t ft2play_LoadModule(const uint8_t *moduleData, uint32_t dataLength);
** int8_t ft2play_Init(uint32_t outputFreq, int8_t lerpMixFlag, int8_t volRampFlag);
** void ft2play_FreeSong(void);
** void ft2play_Close(void);
** void ft2play_PauseSong(int8_t pause);
** void ft2play_PlaySong(void);
*/

#define AUDIO_BUF_LEN 4096

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "stdint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>

#ifndef true
#define true 1
#define false 0
#endif

#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#define LERP(x, y, z) ((x) + ((y) - (x)) * (z))

/* fast 32-bit -> 16-bit clamp */
#define CLAMP16(i) \
{ \
    if ((int16_t)(i) != i) \
        i = 0x7FFF ^ (i >> 31); \
}

enum
{
    IS_Vol      = 1,
    IS_Period   = 2,
    IS_NyTon    = 4,
    IS_Pan      = 8,
    IS_QuickVol = 16
};

/* *** STRUCTS *** (remember 1-byte alignment for header/loader structs) */
#ifdef _MSC_VER
#pragma pack(push)
#pragma pack(1)
#endif
typedef struct songHeaderTyp_t
{
    char sig[17], name[21], progName[20];
    uint16_t ver;
    int32_t headerSize;
    uint16_t len, repS, antChn, antPtn, antInstrs, flags, defTempo, defSpeed;
    uint8_t songTab[256];
}
#ifdef __GNUC__
__attribute__ ((packed))
#endif
songHeaderTyp;

typedef struct sampleHeaderTyp_t
{
    int32_t len, repS, repL;
    uint8_t vol;
    int8_t fine;
    uint8_t typ, pan;
    int8_t relTon;
    uint8_t skrap;
    char name[22];
}
#ifdef __GNUC__
__attribute__ ((packed))
#endif
sampleHeaderTyp;

typedef struct instrHeaderTyp_t
{
    int32_t instrSize;
    char name[22];
    uint8_t typ;
    uint16_t antSamp;
    int32_t sampleSize;
    uint8_t ta[96];
    int16_t envVP[12][2], envPP[12][2];
    uint8_t envVPAnt, envPPAnt, envVSust, envVRepS, envVRepE, envPSust, envPRepS;
    uint8_t envPRepE, envVTyp, envPTyp, vibTyp, vibSweep, vibDepth, vibRate;
    uint16_t fadeOut;
    uint8_t midiOn, midiChannel;
    int16_t midiProgram, midiBend;
    int8_t mute;
    uint8_t reserved[15];
    sampleHeaderTyp samp[32];
}
#ifdef __GNUC__
__attribute__ ((packed))
#endif
instrHeaderTyp;

typedef struct patternHeaderTyp_t
{
    int32_t patternHeaderSize;
    uint8_t typ;
    uint16_t pattLen, dataLen;
}
#ifdef __GNUC__
__attribute__ ((packed))
#endif
patternHeaderTyp;
#ifdef _MSC_VER
#pragma pack(pop)
#endif

typedef struct songTyp_t
{
    uint16_t len, repS;
    uint8_t antChn;
    uint16_t antPtn, antInstrs;
    int16_t songPos, pattNr, pattPos, pattLen;
    uint16_t speed, tempo, globVol, timer;
    uint8_t pattDelTime, pattDelTime2, pBreakFlag, pBreakPos, posJumpFlag, songTab[256];
    uint16_t ver;
    char name[21];
} songTyp;

typedef struct sampleTyp_t
{
    int32_t len, repS, repL;
    uint8_t vol;
    int8_t fine;
    uint8_t typ, pan;
    int8_t relTon;
    uint8_t skrap;
    char name[22];
    int8_t *pek;
    int32_t gusBase, gusLen;
    uint8_t fixed;
    uint16_t fixSpar;
    int32_t fixedPos;
} sampleTyp;

typedef struct instrTyp_t
{
    uint8_t ta[96];
    int16_t envVP[12][2], envPP[12][2];
    uint8_t envVPAnt, envPPAnt, envVSust, envVRepS, envVRepE, envPSust, envPRepS;
    uint8_t envPRepE, envVTyp, envPTyp, vibTyp, vibSweep, vibDepth, vibRate;
    uint16_t fadeOut;
    uint8_t midiOn, midiChannel;
    int16_t midiProgram, midiBend;
    uint8_t mute, reserved[15];
    int16_t antSamp;
    sampleTyp samp[32];
} instrTyp;

typedef struct stmTyp_t
{
    int8_t outVol, realVol, relTonNr, fineTune;
    uint8_t oldVol, oldPan, outPan, finalPan, envSustainActive, effTyp, eff;
    uint8_t smpOffset, waveCtrl, status, portaDir, glissFunk, vibPos, tremPos;
    uint8_t vibSpeed, vibDepth, tremSpeed, tremDepth, pattPos, loopCnt;
    uint8_t volSlideSpeed, fVolSlideUpSpeed, fVolSlideDownSpeed, fPortaUpSpeed;
    uint8_t fPortaDownSpeed, ePortaUpSpeed, ePortaDownSpeed, portaUpSpeed;
    uint8_t portaDownSpeed, retrigSpeed, retrigCnt, retrigVol, volKolVol, tonNr;
    uint8_t envVPos, eVibPos, envPPos, tremorSave, tremorPos, globVolSlideSpeed;
    uint8_t panningSlideSpeed, mute;
    int16_t realPeriod, envVIPValue, envPIPValue;
    uint16_t smpStartPos, instrNr, tonTyp, finalVol, fadeOutSpeed, envVCnt, envVAmp;
    uint16_t envPCnt, envPAmp, eVibAmp, eVibSweep, portaSpeed, wantPeriod, finalPeriod;
    uint16_t outPeriod;
    int32_t fadeOutAmp;
    sampleTyp *smpPtr;
    instrTyp *instrPtr;
} stmTyp;

typedef struct tonTyp_t
{
    uint8_t ton, instr, vol, effTyp, eff;
} tonTyp;

typedef struct
{
    const int8_t *sampleData8, *sampleData8R;
    const int16_t *sampleData16, *sampleData16R;
    int8_t loop, sample16bit, stereo, loopingBackwards, setNewSmp;
    int32_t sampleLength, sampleLoopBegin, sampleLoopEnd, samplePosition, sampleLoopLength;
    float delta, frac, volume, volumeL, volumeR, sampleL, sampleR, targetVolR, targetVolL;
    float panL, panR, volDeltaL, volDeltaR, fader, faderDelta, faderDest, nextSampleL;
    float nextSampleR;
} voice_t;

typedef struct
{
    uint8_t *_ptr, *_base;
    int32_t _eof;
    uint32_t _cnt,  _bufsiz;
} MEM;

#define MAX_NOTES ((12 * 10 * 16) + 16)
#define MAX_VOICES 127


/* TABLES AND VARIABLES */
static const uint32_t panningTab[257] =
{
        0, 4096, 5793, 7094, 8192, 9159,10033,10837,11585,12288,12953,13585,14189,14768,15326,15864,
    16384,16888,17378,17854,18318,18770,19212,19644,20066,20480,20886,21283,21674,22058,22435,22806,
    23170,23530,23884,24232,24576,24915,25249,25580,25905,26227,26545,26859,27170,27477,27780,28081,
    28378,28672,28963,29251,29537,29819,30099,30377,30652,30924,31194,31462,31727,31991,32252,32511,
    32768,33023,33276,33527,33776,34024,34270,34514,34756,34996,35235,35472,35708,35942,36175,36406,
    36636,36864,37091,37316,37540,37763,37985,38205,38424,38642,38858,39073,39287,39500,39712,39923,
    40132,40341,40548,40755,40960,41164,41368,41570,41771,41972,42171,42369,42567,42763,42959,43154,
    43348,43541,43733,43925,44115,44305,44494,44682,44869,45056,45242,45427,45611,45795,45977,46160,
    46341,46522,46702,46881,47059,47237,47415,47591,47767,47942,48117,48291,48465,48637,48809,48981,
    49152,49322,49492,49661,49830,49998,50166,50332,50499,50665,50830,50995,51159,51323,51486,51649,
    51811,51972,52134,52294,52454,52614,52773,52932,53090,53248,53405,53562,53719,53874,54030,54185,
    54340,54494,54647,54801,54954,55106,55258,55410,55561,55712,55862,56012,56162,56311,56459,56608,
    56756,56903,57051,57198,57344,57490,57636,57781,57926,58071,58215,58359,58503,58646,58789,58931,
    59073,59215,59357,59498,59639,59779,59919,60059,60199,60338,60477,60615,60753,60891,61029,61166,
    61303,61440,61576,61712,61848,61984,62119,62254,62388,62523,62657,62790,62924,63057,63190,63323,
    63455,63587,63719,63850,63982,64113,64243,64374,64504,64634,64763,64893,65022,65151,65279,65408,
    65536
};

static uint16_t amigaFinePeriod[12 * 8] =
{
    907,900,894,887,881,875,868,862,856,850,844,838,
    832,826,820,814,808,802,796,791,785,779,774,768,
    762,757,752,746,741,736,730,725,720,715,709,704,
    699,694,689,684,678,675,670,665,660,655,651,646,
    640,636,632,628,623,619,614,610,604,601,597,592,
    588,584,580,575,570,567,563,559,555,551,547,543,
    538,535,532,528,524,520,516,513,508,505,502,498,
    494,491,487,484,480,477,474,470,467,463,460,457
};

static uint8_t vibTab[32] =
{
    0,  24,  49, 74, 97,120,141,161,
    180,197,212,224,235,244,250,253,
    255,253,250,244,235,224,212,197,
    180,161,141,120, 97, 74, 49, 24
};

/* PRE-ZEROED VARIABLES */
static volatile int8_t mixingMutex, isMixing;
static int8_t linearFrqTab, samplingInterpolation, volumeRamping;
static uint16_t pattLens[256];
static int32_t pmpLeft, soundBufferSize;
static uint32_t speedVal, realReplayRate;
static float f_audioFreq, quickVolRampMul_f, tickVolRampMul_f;
static songTyp song;
static tonTyp *patt[256];
static stmTyp stm[MAX_VOICES];
static tonTyp nilPatternLine;
static instrTyp *instr[255 + 1];
static voice_t voice[MAX_VOICES * 2];

/* pre-NULL'd pointers */
static int8_t *vibSineTab     = NULL;
static int16_t *linearPeriods = NULL, *amigaPeriods = NULL, *note2Period = NULL;
static uint32_t *logTab       = NULL;
static float *mixerBufferL    = NULL, *mixerBufferR = NULL;

/* globally accessed */
int8_t moduleLoaded = false, musicPaused  = false, songPlaying  = false;

/* FUNCTION DECLARATIONS */
static MEM *mopen(const uint8_t *src, uint32_t length);
static void mclose(MEM **buf);
static size_t mread(void *buffer, size_t size, size_t count, MEM *buf);
static int32_t meof(MEM *buf);
static void mseek(MEM *buf, int32_t offset, int32_t whence);
void ft2play_FreeSong(void);


/* CODE START */
static void setSpeed(uint16_t bpm)
{
    if (bpm > 0)
    {
        speedVal = ((realReplayRate * 5) / 2) / bpm;
        tickVolRampMul_f = 1.0f / speedVal;
    }
}

static void retrigVolume(stmTyp *ch)
{
    ch->realVol = ch->oldVol;
    ch->outVol  = ch->oldVol;
    ch->outPan  = ch->oldPan;
    ch->status |= (IS_Vol + IS_Pan + IS_QuickVol);
}

static void retrigEnvelopeVibrato(stmTyp *ch)
{
    instrTyp *ins;

    if (!(ch->waveCtrl & 0x04)) ch->vibPos  = 0;
    if (!(ch->waveCtrl & 0x40)) ch->tremPos = 0;

    ch->retrigCnt = 0;
    ch->tremorPos = 0;

    ch->envSustainActive = true;

    ins = ch->instrPtr;

    if (ins->envVTyp & 1)
    {
        ch->envVCnt = 65535;
        ch->envVPos = 0;
    }

    if (ins->envPTyp & 1)
    {
        ch->envPCnt = 65535;
        ch->envPPos = 0;
    }

    ch->fadeOutSpeed = ins->fadeOut; /* FT2 doesn't check if fadeout is more than 4095 */
    ch->fadeOutAmp   = 32768;

    if (ins->vibDepth != 0)
    {
        ch->eVibPos = 0;

        if (ins->vibSweep != 0)
        {
            ch->eVibAmp   = 0;
            ch->eVibSweep = (ins->vibDepth << 8) / ins->vibSweep;
        }
        else
        {
            ch->eVibAmp   = ins->vibDepth << 8;
            ch->eVibSweep = 0;
        }
    }
}

static void keyOff(stmTyp *ch)
{
    instrTyp *ins;

    ch->envSustainActive = false;

    ins = ch->instrPtr;

    if (!(ins->envPTyp & 1)) /* yes, FT2 does this (!) */
    {
        if (ch->envPCnt >= ins->envPP[ch->envPPos][0])
            ch->envPCnt  = ins->envPP[ch->envPPos][0] - 1;
    }

    if (ins->envVTyp & 1)
    {
        if (ch->envVCnt >= ins->envVP[ch->envVPos][0])
            ch->envVCnt  = ins->envVP[ch->envVPos][0] - 1;
    }
    else
    {
        ch->realVol = 0;
        ch->outVol  = 0;
        ch->status |= (IS_Vol + IS_QuickVol);
    }
}

static uint32_t getFrequenceValue(uint16_t period)
{
    uint8_t shift;
    uint16_t index;
    uint32_t rate;

    if (period == 0)
        return (0);

    if (linearFrqTab)
    {
        index = (12 * 192 * 4) - period;
        rate  = logTab[index % (12 * 16 * 4)];

        shift = (14 - (index / (12 * 16 * 4))) & 0x1F;
        if (shift > 0)
            rate >>= shift;
    }
    else
    {
        rate = (1712 * 8363) / period;
    }

    return (rate);
}

static void startTone(uint8_t ton, uint8_t effTyp, uint8_t eff, stmTyp *ch)
{
    uint8_t smp;
    uint16_t tmpTon;
    sampleTyp *s;
    instrTyp *ins;

    /* no idea if this EVER triggers... */
    if (ton == 97)
    {
        keyOff(ch);
        return;
    }
    /* ------------------------------------------------------------ */

    /* if we came from Rxy (retrig), we didn't check note (Ton) yet */
    if (ton == 0)
    {
        ton = ch->tonNr;
        if (ton == 0) return; /* if still no note, return. */
    }
    /* ------------------------------------------------------------ */

    ch->tonNr = ton;

    if (instr[ch->instrNr] != NULL)
        ins = instr[ch->instrNr];
    else
        ins = instr[0]; /* placeholder for invalid samples */

    ch->instrPtr = ins;

    ch->mute = ins->mute;

    if (ton > 96) /* added security check */
        ton = 96;

    smp = ins->ta[ton - 1] & 0x0F;

    s = &ins->samp[smp];
    ch->smpPtr = s;

    ch->relTonNr = s->relTon;

    ton += ch->relTonNr;
    if (ton >= (12 * 10))
        return;

    ch->oldVol = s->vol;
    ch->oldPan = s->pan;

    if ((effTyp == 0x0E) && ((eff & 0xF0) == 0x50))
        ch->fineTune = ((eff & 0x0F) * 16) - 128; /* result is now -128 .. 127 */
    else
        ch->fineTune = s->fine;

    if (ton > 0)
    {
        /* MUST use >> 3 here (sar cl,3) - safe for x86/x86_64 */
        tmpTon = ((ton - 1) * 16) + (((ch->fineTune >> 3) + 16) & 0xFF);

        if (tmpTon < MAX_NOTES) /* should never happen, but FT2 does this check */
        {
            ch->realPeriod = note2Period[tmpTon];
            ch->outPeriod  = ch->realPeriod;
        }
    }

    ch->status |= (IS_Period + IS_Vol + IS_Pan + IS_NyTon + IS_QuickVol);

    if (effTyp == 9)
    {
        if (eff)
            ch->smpOffset = ch->eff;

        ch->smpStartPos = 256 * ch->smpOffset;
    }
    else
    {
        ch->smpStartPos = 0;
    }
}

static void multiRetrig(stmTyp *ch)
{
    int8_t cmd;
    uint8_t cnt;
    int16_t vol;

    cnt = ch->retrigCnt + 1;
    if (cnt < ch->retrigSpeed)
    {
        ch->retrigCnt = cnt;
        return;
    }

    ch->retrigCnt = 0;

    vol = ch->realVol;
    cmd = ch->retrigVol;

    /* 0x00 and 0x08 are not handled, ignore them */

    if (cmd == 0x01)
    {
        vol -= 1;
        if (vol < 0) vol = 0;
    }
    else if (cmd == 0x02)
    {
        vol -= 2;
        if (vol < 0) vol = 0;
    }
    else if (cmd == 0x03)
    {
        vol -= 4;
        if (vol < 0) vol = 0;
    }
    else if (cmd == 0x04)
    {
        vol -= 8;
        if (vol < 0) vol = 0;
    }
    else if (cmd == 0x05)
    {
        vol -= 16;
        if (vol < 0) vol = 0;
    }
    else if (cmd == 0x06)
    {
        vol = (vol >> 1) + (vol >> 3) + (vol >> 4);
    }
    else if (cmd == 0x07)
    {
        vol >>= 1;
    }
    else if (cmd == 0x09)
    {
        vol += 1;
        if (vol > 64) vol = 64;
    }
    else if (cmd == 0x0A)
    {
        vol += 2;
        if (vol > 64) vol = 64;
    }
    else if (cmd == 0x0B)
    {
        vol += 4;
        if (vol > 64) vol = 64;
    }
    else if (cmd == 0x0C)
    {
        vol += 8;
        if (vol > 64) vol = 64;
    }
    else if (cmd == 0x0D)
    {
        vol += 16;
        if (vol > 64) vol = 64;
    }
    else if (cmd == 0x0E)
    {
        vol = (vol >> 1) + vol;
        if (vol > 64) vol = 64;
    }
    else if (cmd == 0x0F)
    {
        vol += vol;
        if (vol > 64) vol = 64;
    }

    ch->realVol = (uint8_t)(vol);
    ch->outVol  = ch->realVol;

    if ((ch->volKolVol >= 0x10) && (ch->volKolVol <= 0x50))
    {
        ch->outVol  = ch->volKolVol - 0x10;
        ch->realVol = ch->outVol;
    }
    else if ((ch->volKolVol >= 0xC0) && (ch->volKolVol <= 0xCF))
    {
        ch->outPan = (ch->volKolVol & 0x0F) << 4;
    }

    startTone(0, 0, 0, ch);
}

static void checkMoreEffects(stmTyp *ch)
{
    int8_t envPos, envUpdate;
    uint8_t tmpEff;
    int16_t newEnvPos;
    uint16_t i;
    instrTyp *ins;

    ins = ch->instrPtr;

    /* Bxx - position jump */
    if (ch->effTyp == 11)
    {
        song.songPos     = (int16_t)(ch->eff) - 1;
        song.pBreakPos   = 0;
        song.posJumpFlag = true;
    }

    /* Dxx - pattern break */
    else if (ch->effTyp == 13)
    {
        song.posJumpFlag = true;

        tmpEff = ((ch->eff >> 4) * 10) + (ch->eff & 0x0F);
        if (tmpEff <= 63)
            song.pBreakPos = tmpEff;
        else
            song.pBreakPos = 0;
    }

    /* Exx - E effects */
    else if (ch->effTyp == 14)
    {
        /* E1x - fine period slide up */
        if ((ch->eff & 0xF0) == 0x10)
        {
            tmpEff = ch->eff & 0x0F;
            if (!tmpEff)
                tmpEff = ch->fPortaUpSpeed;

            ch->fPortaUpSpeed = tmpEff;

            ch->realPeriod -= (tmpEff * 4);
            if (ch->realPeriod < 1) ch->realPeriod = 1;

            ch->outPeriod = ch->realPeriod;
            ch->status   |= IS_Period;
        }

        /* E2x - fine period slide down */
        else if ((ch->eff & 0xF0) == 0x20)
        {
            tmpEff = ch->eff & 0x0F;
            if (!tmpEff)
                tmpEff = ch->fPortaDownSpeed;

            ch->fPortaDownSpeed = tmpEff;

            ch->realPeriod += (tmpEff * 4);
            if (ch->realPeriod > (32000 - 1)) ch->realPeriod = 32000 - 1;

            ch->outPeriod = ch->realPeriod;
            ch->status   |= IS_Period;
        }

        /* E3x - set glissando type */
        else if ((ch->eff & 0xF0) == 0x30) ch->glissFunk = ch->eff & 0x0F;

        /* E4x - set vibrato waveform */
        else if ((ch->eff & 0xF0) == 0x40) ch->waveCtrl = (ch->waveCtrl & 0xF0) | (ch->eff & 0x0F);

        /* E5x (set finetune) is handled in StartTone() */

        /* E6x - pattern loop */
        else if ((ch->eff & 0xF0) == 0x60)
        {
            if (ch->eff == 0x60) /* E60, empty param */
            {
                ch->pattPos = song.pattPos & 0x00FF;
            }
            else
            {
                if (!ch->loopCnt)
                {
                    ch->loopCnt = ch->eff & 0x0F;

                    song.pBreakPos  = ch->pattPos;
                    song.pBreakFlag = true;
                }
                else
                {
                    if (--ch->loopCnt)
                    {
                        song.pBreakPos  = ch->pattPos;
                        song.pBreakFlag = true;
                    }
                }
            }
        }

        /* E7x - set tremolo waveform */
        else if ((ch->eff & 0xF0) == 0x70) ch->waveCtrl = ((ch->eff & 0x0F) << 4) | (ch->waveCtrl & 0x0F);

        /* E8x - set 4-bit panning (NON-FT2) */
        else if ((ch->eff & 0xF0) == 0x80)
        {
            ch->outPan  = (ch->eff & 0x0F) * 16;
            ch->status |= IS_Pan;
        }

        /* EAx - fine volume slide up */
        else if ((ch->eff & 0xF0) == 0xA0)
        {
            tmpEff = ch->eff & 0x0F;
            if (!tmpEff)
                tmpEff = ch->fVolSlideUpSpeed;

            ch->fVolSlideUpSpeed = tmpEff;

            /* unsigned clamp */
            if (ch->realVol <= (64 - tmpEff))
                ch->realVol += tmpEff;
            else
                ch->realVol = 64;

            ch->outVol = ch->realVol;
            ch->status |= IS_Vol;
        }

        /* EBx - fine volume slide down */
        else if ((ch->eff & 0xF0) == 0xB0)
        {
            tmpEff = ch->eff & 0x0F;
            if (!tmpEff)
                tmpEff = ch->fVolSlideDownSpeed;

            ch->fVolSlideDownSpeed = tmpEff;

            /* unsigned clamp */
            if (ch->realVol >= tmpEff)
                ch->realVol -= tmpEff;
            else
                ch->realVol = 0;

            ch->outVol = ch->realVol;
            ch->status |= IS_Vol;
        }

        /* ECx - note cut */
        else if ((ch->eff & 0xF0) == 0xC0)
        {
            if (ch->eff == 0xC0) /* empty param */
            {
                ch->realVol = 0;
                ch->outVol = 0;
                ch->status |= (IS_Vol + IS_QuickVol);
            }
        }

        /* EEx - pattern delay */
        else if ((ch->eff & 0xF0) == 0xE0)
        {
            if (!song.pattDelTime2)
                song.pattDelTime = (ch->eff & 0x0F) + 1;
        }
    }

    /* Fxx - set speed/tempo */
    else if (ch->effTyp == 15)
    {
        if (ch->eff >= 32)
        {
            song.speed = ch->eff;
            setSpeed(song.speed);
        }
        else
        {
            song.tempo = ch->eff;
            song.timer = ch->eff;
        }
    }

    /* Gxx - set global volume */
    else if (ch->effTyp == 16)
    {
        song.globVol = ch->eff;
        if (song.globVol > 64) song.globVol = 64;

        for (i = 0; i < song.antChn; ++i)
            stm[i].status |= IS_Vol;
    }

    /* Lxx - set vol and pan envelope position */
    else if (ch->effTyp == 21)
    {
        /* *** VOLUME ENVELOPE *** */
        if (ins->envVTyp & 1)
        {
            ch->envVCnt = ch->eff - 1;

            envPos = 0;
            envUpdate = true;
            newEnvPos = ch->eff;

            if (ins->envVPAnt > 1)
            {
                envPos++;
                for (i = 0; i < ins->envVPAnt - 1; ++i)
                {
                    if (newEnvPos < ins->envVP[envPos][0])
                    {
                        envPos--;

                        newEnvPos -= ins->envVP[envPos][0];
                        if (newEnvPos == 0)
                        {
                            envUpdate = false;
                            break;
                        }

                        if (ins->envVP[envPos + 1][0] <= ins->envVP[envPos][0])
                        {
                            envUpdate = true;
                            break;
                        }

                        ch->envVIPValue = ((ins->envVP[envPos + 1][1] - ins->envVP[envPos][1]) & 0x00FF) << 8;
                        ch->envVIPValue /= (ins->envVP[envPos + 1][0] - ins->envVP[envPos][0]);

                        ch->envVAmp = (ch->envVIPValue * (newEnvPos - 1)) + ((ins->envVP[envPos][1] & 0x00FF) << 8);

                        envPos++;

                        envUpdate = false;
                        break;
                    }

                    envPos++;
                }

                if (envUpdate) envPos--;
            }

            if (envUpdate)
            {
                ch->envVIPValue = 0;
                ch->envVAmp = (ins->envVP[envPos][1] & 0x00FF) << 8;
            }

            if (envPos >= ins->envVPAnt)
            {
                envPos = ins->envVPAnt - 1;
                if (envPos < 0)
                    envPos = 0;
            }

            ch->envVPos = envPos;
        }

        /* *** PANNING ENVELOPE *** */
        if (ins->envVTyp & 2) /* probably an FT2 bug */
        {
            ch->envPCnt = ch->eff - 1;

            envPos = 0;
            envUpdate = true;
            newEnvPos = ch->eff;

            if (ins->envPPAnt > 1)
            {
                envPos++;
                for (i = 0; i < ins->envPPAnt - 1; ++i)
                {
                    if (newEnvPos < ins->envPP[envPos][0])
                    {
                        envPos--;

                        newEnvPos -= ins->envPP[envPos][0];
                        if (newEnvPos == 0)
                        {
                            envUpdate = false;
                            break;
                        }

                        if (ins->envPP[envPos + 1][0] <= ins->envPP[envPos][0])
                        {
                            envUpdate = true;
                            break;
                        }

                        ch->envPIPValue = ((ins->envPP[envPos + 1][1] - ins->envPP[envPos][1]) & 0x00FF) << 8;
                        ch->envPIPValue /= (ins->envPP[envPos + 1][0] - ins->envPP[envPos][0]);

                        ch->envPAmp = (ch->envPIPValue * (newEnvPos - 1)) + ((ins->envPP[envPos][1] & 0x00FF) << 8);

                        envPos++;

                        envUpdate = false;
                        break;
                    }

                    envPos++;
                }

                if (envUpdate) envPos--;
            }

            if (envUpdate)
            {
                ch->envPIPValue = 0;
                ch->envPAmp = (ins->envPP[envPos][1] & 0x00FF) << 8;
            }

            if (envPos >= ins->envPPAnt)
            {
                envPos = ins->envPPAnt - 1;
                if (envPos < 0)
                    envPos = 0;
            }

            ch->envPPos = envPos;
        }
    }
}

static void checkEffects(stmTyp *ch)
{
    uint8_t tmpEff, tmpEffHi, volKol;

    /* this one is manipulated by vol column effects, then used for multiretrig (FT2 quirk) */
    volKol = ch->volKolVol;

    /* *** VOLUME COLUMN EFFECTS (TICK 0) *** */

    /* set volume */
    if ((ch->volKolVol >= 0x10) && (ch->volKolVol <= 0x50))
    {
        volKol -= 16;

        ch->outVol  = volKol;
        ch->realVol = volKol;

        ch->status |= (IS_Vol + IS_QuickVol);
    }

    /* fine volume slide down */
    else if ((ch->volKolVol & 0xF0) == 0x80)
    {
        volKol = ch->volKolVol & 0x0F;

        /* unsigned clamp */
        if (ch->realVol >= volKol)
            ch->realVol -= volKol;
        else
            ch->realVol = 0;

        ch->outVol  = ch->realVol;
        ch->status |= IS_Vol;
    }

    /* fine volume slide up */
    else if ((ch->volKolVol & 0xF0) == 0x90)
    {
        volKol = ch->volKolVol & 0x0F;

        /* unsigned clamp */
        if (ch->realVol <= (64 - volKol))
            ch->realVol += volKol;
        else
            ch->realVol = 64;

        ch->outVol  = ch->realVol;
        ch->status |= IS_Vol;
    }

    /* set vibrato speed */
    else if ((ch->volKolVol & 0xF0) == 0xA0)
    {
        volKol = (ch->volKolVol & 0x0F) << 2;
        ch->vibSpeed = volKol;
    }

    /* set panning */
    else if ((ch->volKolVol & 0xF0) == 0xC0)
    {
        volKol <<= 4;

        ch->outPan  = volKol;
        ch->status |= IS_Pan;
    }


    /* *** MAIN EFFECTS (TICK 0) *** */


    if ((ch->effTyp == 0) && (ch->eff == 0)) return;

    /* Cxx - set volume */
    if (ch->effTyp == 12)
    {
        ch->realVol = ch->eff;
        if (ch->realVol > 64) ch->realVol = 64;

        ch->outVol = ch->realVol;
        ch->status |= (IS_Vol + IS_QuickVol);

        return;
    }

    /* 8xx - set panning */
    else if (ch->effTyp == 8)
    {
        ch->outPan  = ch->eff;
        ch->status |= IS_Pan;

        return;
    }

    /* Rxy - note multi retrigger */
    else if (ch->effTyp == 27)
    {
        tmpEff = ch->eff & 0x0F;
        if (!tmpEff)
            tmpEff = ch->retrigSpeed;

        ch->retrigSpeed = tmpEff;

        tmpEffHi = ch->eff >> 4;
        if (!tmpEffHi)
            tmpEffHi = ch->retrigVol;

        ch->retrigVol = tmpEffHi;

        if (volKol == 0)
            multiRetrig(ch);

        return;
    }

    /* X1x - extra fine period slide up */
    else if ((ch->effTyp == 33) && ((ch->eff & 0xF0) == 0x10))
    {
        tmpEff = ch->eff & 0x0F;
        if (!tmpEff)
            tmpEff = ch->ePortaUpSpeed;

        ch->ePortaUpSpeed = tmpEff;

        ch->realPeriod -= tmpEff;
        if (ch->realPeriod < 1) ch->realPeriod = 1;

        ch->outPeriod = ch->realPeriod;
        ch->status |= IS_Period;

        return;
    }

    /* X2x - extra fine period slide down */
    else if ((ch->effTyp == 33) && ((ch->eff & 0xF0) == 0x20))
    {
        tmpEff = ch->eff & 0x0F;
        if (!tmpEff)
            tmpEff = ch->ePortaDownSpeed;

        ch->ePortaDownSpeed = tmpEff;

        ch->realPeriod += tmpEff;
        if (ch->realPeriod > (32000 - 1)) ch->realPeriod = 32000 - 1;

        ch->outPeriod = ch->realPeriod;
        ch->status |= IS_Period;

        return;
    }

    checkMoreEffects(ch);
}

static void fixTonePorta(stmTyp *ch, tonTyp *p, uint8_t inst)
{
    uint16_t portaTmp;

    if (p->ton)
    {
        if (p->ton == 97)
        {
            keyOff(ch);
        }
        else
        {
            /* MUST use >> 3 here (sar cl,3) - safe for x86/x86_64 */
            portaTmp = ((((p->ton - 1) + ch->relTonNr) & 0x00FF) * 16) + (((ch->fineTune >> 3) + 16) & 0x00FF);

            if (portaTmp < MAX_NOTES)
            {
                ch->wantPeriod = note2Period[portaTmp];

                     if (ch->wantPeriod == ch->realPeriod) ch->portaDir = 0;
                else if (ch->wantPeriod  > ch->realPeriod) ch->portaDir = 1;
                else                                       ch->portaDir = 2;
            }
        }
    }

    if (inst)
    {
        retrigVolume(ch);

        if (p->ton != 97)
            retrigEnvelopeVibrato(ch);
    }
}

static void getNewNote(stmTyp *ch, tonTyp *p)
{
    /* this is a mess, but it appears to be 100% FT2-correct */

    uint8_t inst, checkEfx;

    ch->volKolVol = p->vol;

    if (!ch->effTyp)
    {
        if (ch->eff)
        {
            /* we have an arpeggio running, set period back */
            ch->outPeriod = ch->realPeriod;
            ch->status   |= IS_Period;
        }
    }
    else
    {
        if ((ch->effTyp == 4) || (ch->effTyp == 6))
        {
            /* we have a vibrato running */
            if ((p->effTyp != 4) && (p->effTyp != 6))
            {
                /* but it's ending at the next (this) row, so set period back */
                ch->outPeriod = ch->realPeriod;
                ch->status   |= IS_Period;
            }
        }
    }

    ch->effTyp = p->effTyp;
    ch->eff    = p->eff;
    ch->tonTyp = (p->instr << 8) | p->ton;

    /* 'inst' var is used for later if checks... */
    inst = p->instr;
    if (inst)
    {
        if (inst <= 128)
            ch->instrNr = inst;
        else
            inst = 0;
    }

    checkEfx = true;
    if (p->effTyp == 0x0E)
    {
        if ((p->eff >= 0xD1) && (p->eff <= 0xDF))
            return; /* we have a note delay (ED1..EDF) */
        else if (p->eff == 0x90)
            checkEfx = false;
    }

    if (checkEfx)
    {
        if ((ch->volKolVol & 0xF0) == 0xF0) /* gxx */
        {
            if (ch->volKolVol & 0x0F)
                ch->portaSpeed = (ch->volKolVol & 0x0F) << 6;

            fixTonePorta(ch, p, inst);

            checkEffects(ch);

            return;
        }

        if ((p->effTyp == 3) || (p->effTyp == 5)) /* 3xx or 5xx */
        {
            if ((p->effTyp != 5) && (p->eff != 0))
                ch->portaSpeed = p->eff << 2;

            fixTonePorta(ch, p, inst);

            checkEffects(ch);

            return;
        }

        if ((p->effTyp == 0x14) && (p->eff == 0)) /* K00 (KeyOff - only handle tick 0 here) */
        {
            keyOff(ch);

            if (inst)
                retrigVolume(ch);

            checkEffects(ch);
            return;
        }

        if (!p->ton)
        {
            if (inst)
            {
                retrigVolume(ch);
                retrigEnvelopeVibrato(ch);
            }

            checkEffects(ch);
            return;
        }
    }

    if (p->ton == 97)
        keyOff(ch);
    else
        startTone(p->ton, p->effTyp, p->eff, ch);

    if (inst)
    {
        retrigVolume(ch);

        if (p->ton != 97)
            retrigEnvelopeVibrato(ch);
    }

    checkEffects(ch);
}

static void fixaEnvelopeVibrato(stmTyp *ch)
{
    int8_t envInterpolateFlag, envDidInterpolate;
    uint8_t envPos;
    int16_t autoVibVal, panTmp;
    uint16_t autoVibAmp, tmpPeriod, envVal;
    int32_t tmp32;
    instrTyp *ins;

    ins = ch->instrPtr;

    /* *** FADEOUT *** */
    if (!ch->envSustainActive)
    {
        ch->status |= IS_Vol;

        /* unsigned clamp + reset */
        if (ch->fadeOutAmp >= ch->fadeOutSpeed)
        {
            ch->fadeOutAmp -= ch->fadeOutSpeed;
        }
        else
        {
            ch->fadeOutAmp   = 0;
            ch->fadeOutSpeed = 0;
        }
    }

    if (ch->mute != 1)
    {
        /* *** VOLUME ENVELOPE *** */
        envVal = 0;
        if (ins->envVTyp & 1)
        {
            envDidInterpolate = false;
            envPos = ch->envVPos;

            if (++ch->envVCnt == ins->envVP[envPos][0])
            {
                ch->envVAmp = (ins->envVP[envPos][1] & 0x00FF) << 8;

                envPos++;
                if (ins->envVTyp & 4)
                {
                    envPos--;

                    if (envPos == ins->envVRepE)
                    {
                        if (!(ins->envVTyp & 2) || (envPos != ins->envVSust) || ch->envSustainActive)
                        {
                            envPos = ins->envVRepS;

                            ch->envVCnt =  ins->envVP[envPos][0];
                            ch->envVAmp = (ins->envVP[envPos][1] & 0x00FF) << 8;
                        }
                    }

                    envPos++;
                }

                if (envPos < ins->envVPAnt)
                {
                    envInterpolateFlag = true;
                    if ((ins->envVTyp & 2) && ch->envSustainActive)
                    {
                        if ((envPos - 1) == ins->envVSust)
                        {
                            envPos--;
                            ch->envVIPValue = 0;
                            envInterpolateFlag = false;
                        }
                    }

                    if (envInterpolateFlag)
                    {
                        ch->envVPos = envPos;

                        ch->envVIPValue = 0;
                        if (ins->envVP[envPos][0] > ins->envVP[envPos - 1][0])
                        {
                            ch->envVIPValue = ((ins->envVP[envPos][1] - ins->envVP[envPos - 1][1]) & 0x00FF) << 8;
                            ch->envVIPValue /= (ins->envVP[envPos][0] - ins->envVP[envPos - 1][0]);

                            envVal = ch->envVAmp;
                            envDidInterpolate = true;
                        }
                    }
                }
                else
                {
                    ch->envVIPValue = 0;
                }
            }

            if (!envDidInterpolate)
            {
                ch->envVAmp += ch->envVIPValue;

                envVal = ch->envVAmp;
                if ((envVal >> 8) > 0x40)
                {
                    if ((envVal >> 8) > ((0x40 + 0xC0) / 2))
                        envVal = 16384;
                    else
                        envVal = 0;

                    ch->envVIPValue = 0;
                }
            }

            envVal >>= 8;

            ch->finalVol = (song.globVol * (((envVal * ch->outVol) * ch->fadeOutAmp) >> (16 + 2))) >> 7;
            ch->status  |= IS_Vol;
        }
        else
        {
            ch->finalVol = (song.globVol * (((ch->outVol << 4) * ch->fadeOutAmp) >> 16)) >> 7;
        }

        /* non-FT2 ear security system */
        if (ch->finalVol > 256)
            ch->finalVol = 256;
    }
    else
    {
        ch->finalVol = 0;
    }

    /* *** PANNING ENVELOPE *** */

    envVal = 0;
    if (ins->envPTyp & 1)
    {
        envDidInterpolate = false;
        envPos = ch->envPPos;

        if (++ch->envPCnt == ins->envPP[envPos][0])
        {
            ch->envPAmp = (ins->envPP[envPos][1] & 0x00FF) << 8;

            envPos++;
            if (ins->envPTyp & 4)
            {
                envPos--;

                if (envPos == ins->envPRepE)
                {
                    if (!(ins->envPTyp & 2) || (envPos != ins->envPSust) || ch->envSustainActive)
                    {
                        envPos = ins->envPRepS;

                        ch->envPCnt =  ins->envPP[envPos][0];
                        ch->envPAmp = (ins->envPP[envPos][1] & 0x00FF) << 8;
                    }
                }

                envPos++;
            }

            if (envPos < ins->envPPAnt)
            {
                envInterpolateFlag = true;
                if ((ins->envPTyp & 2) && ch->envSustainActive)
                {
                    if ((envPos - 1) == ins->envPSust)
                    {
                        envPos--;
                        ch->envPIPValue = 0;
                        envInterpolateFlag = false;
                    }
                }

                if (envInterpolateFlag)
                {
                    ch->envPPos = envPos;

                    ch->envPIPValue = 0;
                    if (ins->envPP[envPos][0] > ins->envPP[envPos - 1][0])
                    {
                        ch->envPIPValue  = ((ins->envPP[envPos][1] - ins->envPP[envPos - 1][1]) & 0x00FF) << 8;
                        ch->envPIPValue /=  (ins->envPP[envPos][0] - ins->envPP[envPos - 1][0]);

                        envVal = ch->envPAmp;
                        envDidInterpolate = true;
                    }
                }
            }
            else
            {
                ch->envPIPValue = 0;
            }
        }

        if (!envDidInterpolate)
        {
            ch->envPAmp += ch->envPIPValue;

            envVal = ch->envPAmp;
            if ((envVal >> 8) > 0x40)
            {
                if ((envVal >> 8) > ((0x40 + 0xC0) / 2))
                    envVal = 16384;
                else
                    envVal = 0;

                ch->envPIPValue = 0;
            }
        }

        /* panning calculated exactly like FT2 */
        panTmp = ch->outPan - 128;
        if (panTmp > 0) panTmp = 0 - panTmp;
        panTmp += 128;

        envVal -= (32 * 256);

        ch->finalPan = ch->outPan + (int8_t)((((int16_t)(envVal) * (panTmp << 3)) >> 16) & 0xFF);
        ch->status  |= IS_Pan;
    }
    else
    {
        ch->finalPan = ch->outPan;
    }

    /* *** AUTO VIBRATO *** */
    if (ins->vibDepth != 0)
    {
        if (ch->eVibSweep != 0)
        {
            autoVibAmp = ch->eVibSweep;
            if (ch->envSustainActive)
            {
                autoVibAmp += ch->eVibAmp;
                if ((autoVibAmp >> 8) > ins->vibDepth)
                {
                    autoVibAmp    = ins->vibDepth << 8;
                    ch->eVibSweep = 0;
                }

                ch->eVibAmp = autoVibAmp;
            }
        }
        else
        {
            autoVibAmp = ch->eVibAmp;
        }

        ch->eVibPos += ins->vibRate;

        /* square */
        if (ins->vibTyp == 1)
            autoVibVal = (ch->eVibPos > 127) ? 64 : -64;

        /* ramp up */
        else if (ins->vibTyp == 2)
            autoVibVal = (((ch->eVibPos >> 1) + 64) & 127) - 64;

        /* ramp down */
        else if (ins->vibTyp == 3)
            autoVibVal = (((0 - (ch->eVibPos >> 1)) + 64) & 127) - 64;

        /* sine */
        else
            autoVibVal = vibSineTab[ch->eVibPos];

        autoVibVal <<= 2;

        tmp32 = ((autoVibVal * (signed)(autoVibAmp)) >> 16) & 0x8000FFFF;
        tmpPeriod = ch->outPeriod + (int16_t)(tmp32);
        if (tmpPeriod > (32000 - 1)) tmpPeriod = 0; /* yes, FT2 zeroes it out */

        ch->finalPeriod = tmpPeriod;

        ch->status  |= IS_Period;
    }
    else
    {
        ch->finalPeriod = ch->outPeriod;
    }
}

int16_t relocateTon(int16_t period, int8_t relativeNote, stmTyp *ch)
{
    int8_t i, fineTune;
    int16_t *periodTable;
    int32_t loPeriod, hiPeriod, tmpPeriod, tableIndex;

    fineTune    = (ch->fineTune >> 3) + 16; /* MUST use >> 3 here (sar cl,3) - safe for x86/x86_64 */
    hiPeriod    = 8 * 12 * 16;
    loPeriod    = 0;
    periodTable = note2Period;

    for (i = 0; i < 8; ++i)
    {
        tmpPeriod = (((loPeriod + hiPeriod) / 2) & ~15) + fineTune;

        tableIndex = tmpPeriod - 8;
        if (tableIndex < 0) /* added security check */
            tableIndex = 0;

        if (period >= periodTable[tableIndex])
            hiPeriod = tmpPeriod - fineTune;
        else
            loPeriod = tmpPeriod - fineTune;
    }

    tmpPeriod = loPeriod + fineTune + (relativeNote * 16);
    if (tmpPeriod < 0) /* added security check */
        tmpPeriod = 0;

    if (tmpPeriod >= ((8 * 12 * 16) + 15) - 1) /* FT2 bug: stupid off-by-one edge case */
        tmpPeriod  =  (8 * 12 * 16) + 15;

    return (periodTable[tmpPeriod]);
}

static void tonePorta(stmTyp *ch)
{
    if (ch->portaDir)
    {
        if (ch->portaDir > 1)
        {
            ch->realPeriod -= ch->portaSpeed;
            if (ch->realPeriod <= ch->wantPeriod)
            {
                ch->portaDir   = 1;
                ch->realPeriod = ch->wantPeriod;
            }
        }
        else
        {
            ch->realPeriod += ch->portaSpeed;
            if (ch->realPeriod >= ch->wantPeriod)
            {
                ch->portaDir   = 1;
                ch->realPeriod = ch->wantPeriod;
            }
        }

        if (ch->glissFunk) /* semi-tone slide flag */
            ch->outPeriod = relocateTon(ch->realPeriod, 0, ch);
        else
            ch->outPeriod = ch->realPeriod;

        ch->status |= IS_Period;
    }
}

static void volume(stmTyp *ch) /* actually volume slide */
{
    uint8_t tmpEff;

    tmpEff = ch->eff;
    if (!tmpEff)
        tmpEff = ch->volSlideSpeed;

    ch->volSlideSpeed = tmpEff;

    if (!(tmpEff & 0xF0))
    {
        /* unsigned clamp */
        if (ch->realVol >= tmpEff)
            ch->realVol -= tmpEff;
        else
            ch->realVol = 0;
    }
    else
    {
        /* unsigned clamp */
        if (ch->realVol <= (64 - (tmpEff >> 4)))
            ch->realVol += (tmpEff >> 4);
        else
            ch->realVol = 64;
    }

    ch->outVol  = ch->realVol;
    ch->status |= IS_Vol;
}

static void vibrato2(stmTyp *ch)
{
    uint8_t tmpVib;

    tmpVib = (ch->vibPos / 4) & 0x1F;

    switch (ch->waveCtrl & 0x03)
    {
        /* 0: sine */
        case 0:
        {
            tmpVib = vibTab[tmpVib];
        }
        break;

        /* 1: ramp */
        case 1:
        {
            tmpVib *= 8;
            if (ch->vibPos >= 128)
                tmpVib ^= 0xFF;
        }
        break;

        /* 2/3: square */
        default:
        {
            tmpVib = 255;
        }
        break;
    }

    tmpVib = (tmpVib * ch->vibDepth) / 32;

    if (ch->vibPos >= 128)
        ch->outPeriod = ch->realPeriod - tmpVib;
    else
        ch->outPeriod = ch->realPeriod + tmpVib;

    ch->status |= IS_Period;
    ch->vibPos += ch->vibSpeed;
}

static void vibrato(stmTyp *ch)
{
    if (ch->eff)
    {
        if (ch->eff & 0x0F) ch->vibDepth = ch->eff & 0x0F;
        if (ch->eff & 0xF0) ch->vibSpeed = (ch->eff >> 4) * 4;
    }

    vibrato2(ch);
}

static void doEffects(stmTyp *ch)
{
    int8_t note;
    uint8_t tmpEff, tremorData, tremorSign, tmpTrem;
    int16_t tremVol;
    uint16_t i, tick;

    /* *** VOLUME COLUMN EFFECTS (TICKS >0) *** */

    /* volume slide down */
    if ((ch->volKolVol & 0xF0) == 0x60)
    {
        /* unsigned clamp */
        if (ch->realVol >= (ch->volKolVol & 0x0F))
            ch->realVol -= (ch->volKolVol & 0x0F);
        else
            ch->realVol = 0;

        ch->outVol  = ch->realVol;
        ch->status |= IS_Vol;
    }

    /* volume slide up */
    else if ((ch->volKolVol & 0xF0) == 0x70)
    {
        /* unsigned clamp */
        if (ch->realVol <= (64 - (ch->volKolVol & 0x0F)))
            ch->realVol += (ch->volKolVol & 0x0F);
        else
            ch->realVol = 64;

        ch->outVol  = ch->realVol;
        ch->status |= IS_Vol;
    }

    /* vibrato (+ set vibrato depth) */
    else if ((ch->volKolVol & 0xF0) == 0xB0)
    {
        if (ch->volKolVol != 0xB0)
            ch->vibDepth = ch->volKolVol & 0x0F;

        vibrato2(ch);
    }

    /* pan slide left */
    else if ((ch->volKolVol & 0xF0) == 0xD0)
    {
        /* unsigned clamp + a bug when the parameter is 0  */
        if (((ch->volKolVol & 0x0F) == 0) || (ch->outPan < (ch->volKolVol & 0x0F)))
            ch->outPan = 0;
        else
            ch->outPan -= (ch->volKolVol & 0x0F);

        ch->status |= IS_Pan;
    }

    /* pan slide right */
    else if ((ch->volKolVol & 0xF0) == 0xE0)
    {
        /* unsigned clamp */
        if (ch->outPan <= (255 - (ch->volKolVol & 0x0F)))
            ch->outPan += (ch->volKolVol & 0x0F);
        else
            ch->outPan = 255;

        ch->status |= IS_Pan;
    }

    /* tone portamento */
    else if ((ch->volKolVol & 0xF0) == 0xF0) tonePorta(ch);

    /* *** MAIN EFFECTS (TICKS >0) *** */

    if (((ch->eff == 0) && (ch->effTyp == 0)) || (ch->effTyp >= 36)) return;

    /* 0xy - Arpeggio */
    if (ch->effTyp == 0)
    {
        tick = song.timer;
        note = 0;

        /* FT2 'out of boundary' arp LUT simulation */
             if (tick  > 16) tick  = 2;
        else if (tick == 16) tick  = 0;
        else                 tick %= 3;

        /*
        ** this simulation doesn't work properly for >=128 tick arps,
        ** but you'd need to hexedit the initial speed to get >31
        */

        if (!tick)
        {
            ch->outPeriod = ch->realPeriod;
        }
        else
        {
                 if (tick == 1) note = ch->eff >> 4;
            else if (tick >  1) note = ch->eff & 0x0F;

            ch->outPeriod = relocateTon(ch->realPeriod, note, ch);
        }

        ch->status |= IS_Period;
    }

    /* 1xx - period slide up */
    else if (ch->effTyp == 1)
    {
        tmpEff = ch->eff;
        if (!tmpEff)
            tmpEff = ch->portaUpSpeed;

        ch->portaUpSpeed = tmpEff;

        ch->realPeriod -= (tmpEff * 4);
        if (ch->realPeriod < 1) ch->realPeriod = 1;

        ch->outPeriod = ch->realPeriod;
        ch->status   |= IS_Period;
    }

    /* 2xx - period slide down */
    else if (ch->effTyp == 2)
    {
        tmpEff = ch->eff;
        if (!tmpEff)
            tmpEff = ch->portaDownSpeed;

        ch->portaDownSpeed = tmpEff;

        ch->realPeriod += (tmpEff * 4);
        if (ch->realPeriod > (32000 - 1)) ch->realPeriod = 32000 - 1;

        ch->outPeriod = ch->realPeriod;
        ch->status   |= IS_Period;
    }

    /* 3xx - tone portamento */
    else if (ch->effTyp == 3) tonePorta(ch);

    /* 4xy - vibrato */
    else if (ch->effTyp == 4) vibrato(ch);

    /* 5xy - tone portamento + volume slide */
    else if (ch->effTyp == 5)
    {
        tonePorta(ch);
        volume(ch);
    }

    /* 6xy - vibrato + volume slide */
    else if (ch->effTyp == 6)
    {
        vibrato2(ch);
        volume(ch);
    }

    /* 7xy - tremolo */
    else if (ch->effTyp == 7)
    {
        tmpEff = ch->eff;
        if (tmpEff)
        {
            if (tmpEff & 0x0F) ch->tremDepth = tmpEff & 0x0F;
            if (tmpEff & 0xF0) ch->tremSpeed = (tmpEff >> 4) * 4;
        }

        tmpTrem = (ch->tremPos / 4) & 0x1F;

        switch ((ch->waveCtrl >> 4) & 3)
        {
            /* 0: sine */
            case 0:
            {
                tmpTrem = vibTab[tmpTrem];
            }
            break;

            /* 1: ramp */
            case 1:
            {
                tmpTrem *= 8;
                if (ch->vibPos >= 128) tmpTrem ^= 0xFF; /* FT2 bug, should've been TremPos */
            }
            break;

            /* 2/3: square */
            default:
            {
                tmpTrem = 255;
            }
            break;
        }

        tmpTrem = (tmpTrem * ch->tremDepth) / 64;

        if (ch->tremPos >= 128)
        {
            tremVol = ch->realVol - tmpTrem;
            if (tremVol < 0) tremVol = 0;
        }
        else
        {
            tremVol = ch->realVol + tmpTrem;
            if (tremVol > 64) tremVol = 64;
        }

        ch->outVol = tremVol & 0x00FF;

        ch->tremPos += ch->tremSpeed;

        ch->status |= IS_Vol;
    }

    /* Axy - volume slide */
    else if (ch->effTyp == 10) volume(ch); /* actually volume slide */

    /* Exy - E effects */
    else if (ch->effTyp == 14)
    {
        /* E9x - note retrigger */
        if ((ch->eff & 0xF0) == 0x90)
        {
            if (ch->eff != 0x90) /* E90 is handled in getNewNote() */
            {
                if (!((song.tempo - song.timer) % (ch->eff & 0x0F)))
                {
                    startTone(0, 0, 0, ch);
                    retrigEnvelopeVibrato(ch);
                }
            }
        }

        /* ECx - note cut */
        else if ((ch->eff & 0xF0) == 0xC0)
        {
            if (((song.tempo - song.timer) & 0x00FF) == (ch->eff & 0x0F))
            {
                ch->outVol  = 0;
                ch->realVol = 0;
                ch->status |= (IS_Vol + IS_QuickVol);
            }
        }

        /* EDx - note delay */
        else if ((ch->eff & 0xF0) == 0xD0)
        {
            if (((song.tempo - song.timer) & 0x00FF) == (ch->eff & 0x0F))
            {
                startTone(ch->tonTyp & 0x00FF, 0, 0, ch);

                if (ch->tonTyp & 0xFF00)
                    retrigVolume(ch);

                retrigEnvelopeVibrato(ch);

                if ((ch->volKolVol >= 0x10) && (ch->volKolVol <= 0x50))
                {
                    ch->outVol  = ch->volKolVol - 16;
                    ch->realVol = ch->outVol;
                }
                else if ((ch->volKolVol >= 0xC0) && (ch->volKolVol <= 0xCF))
                {
                    ch->outPan = (ch->volKolVol & 0x0F) << 4;
                }
            }
        }
    }

    /* Hxy - global volume slide */
    else if (ch->effTyp == 17)
    {
        tmpEff = ch->eff;
        if (!tmpEff)
            tmpEff = ch->globVolSlideSpeed;

        ch->globVolSlideSpeed = tmpEff;

        if (!(tmpEff & 0xF0))
        {
            /* unsigned clamp */
            if (song.globVol >= tmpEff)
                song.globVol -= tmpEff;
            else
                song.globVol = 0;
        }
        else
        {
            /* unsigned clamp */
            if (song.globVol <= (64 - (tmpEff >> 4)))
                song.globVol += (tmpEff >> 4);
            else
                song.globVol = 64;
        }

        for (i = 0; i < song.antChn; ++i)
            stm[i].status |= IS_Vol;
    }

    /* Kxx - key off */
    else if (ch->effTyp == 20)
    {
        if (((song.tempo - song.timer) & 31) == (ch->eff & 0x0F))
            keyOff(ch);
    }

    /* Pxy - panning slide */
    else if (ch->effTyp == 25)
    {
        tmpEff = ch->eff;
        if (!tmpEff)
            tmpEff = ch->panningSlideSpeed;

        ch->panningSlideSpeed = tmpEff;

        if ((tmpEff & 0xF0) == 0)
        {
            /* unsigned clamp */
            if (ch->outPan >= tmpEff)
                ch->outPan -= tmpEff;
            else
                ch->outPan = 0;
        }
        else
        {
            tmpEff >>= 4;

            /* unsigned clamp */
            if (ch->outPan <= (255 - tmpEff))
                ch->outPan += tmpEff;
            else
                ch->outPan = 255;
        }

        ch->status |= IS_Pan;
    }

    /* Rxy - multi note retrig */
    else if (ch->effTyp == 27) multiRetrig(ch);

    /* Txy - tremor */
    else if (ch->effTyp == 29)
    {
        tmpEff = ch->eff;
        if (!tmpEff)
            tmpEff = ch->tremorSave;

        ch->tremorSave = tmpEff;

        tremorSign = ch->tremorPos & 0x80;
        tremorData = ch->tremorPos & 0x7F;

        tremorData--;
        if (tremorData & 0x80)
        {
            if (tremorSign == 0x80)
            {
                tremorSign = 0x00;
                tremorData = tmpEff & 0x0F;
            }
            else
            {
                tremorSign = 0x80;
                tremorData = tmpEff >> 4;
            }
        }

        ch->tremorPos = tremorData | tremorSign;

        ch->outVol  = tremorSign ? ch->realVol : 0;
        ch->status |= (IS_Vol + IS_QuickVol);
    }
}

static void voiceSetVolRamp(uint8_t chNr)
{
    voice_t *v1, *v2;

    if (volumeRamping)
    {
        v1 = &voice[chNr]; /* curr voice */
        v2 = &voice[MAX_VOICES + chNr]; /* ramp out voice */

        if (v1->sampleData8 != NULL)
        {
            /* copy current voice to ramp out voice */
            memcpy(v2, v1, sizeof (voice_t));

            /* set ramp out voice */
            v2->faderDest  = 0.0f;
            v2->faderDelta = (v2->faderDest - v2->fader) * quickVolRampMul_f;
        }

        /* set ramp in for current voice */
        v1->fader      = 0.0f;
        v1->faderDest  = 1.0f;
        v1->faderDelta = (v1->faderDest - v1->fader) * quickVolRampMul_f;
    }
}

static void voiceUpdateVolumes(uint8_t i, uint8_t status)
{
    float volL_f, volR_f, deltaMul_f;
    voice_t *v;

    v = &voice[i];

    volL_f = v->volume * v->panL;
    volR_f = v->volume * v->panR;

    if (volumeRamping)
    {
        if (!(status & IS_NyTon))
        {
            /* set vol ramp stuff */
            v->targetVolL = volL_f;
            v->targetVolR = volR_f;

            deltaMul_f = (status & IS_QuickVol) ? quickVolRampMul_f : tickVolRampMul_f;

            v->volDeltaL = (v->targetVolL - v->volumeL) * deltaMul_f;
            v->volDeltaR = (v->targetVolR - v->volumeR) * deltaMul_f;
        }
        else
        {
            v->targetVolL = v->volumeL = volL_f;
            v->targetVolR = v->volumeR = volR_f;

            v->volDeltaL = 0.0f;
            v->volDeltaR = 0.0f;
        }
    }
    else
    {
        v->volumeL = volL_f;
        v->volumeR = volR_f;
    }
}

static void voiceSetSource(uint8_t i, const int8_t *sampleData,
    int32_t sampleLength,  int32_t sampleLoopBegin, int32_t sampleLoopLength,
    int32_t sampleLoopEnd, int8_t loopFlag, int8_t sixteenbit, int8_t stereo,
    int32_t position)
{
    voice_t *v;

    v = &voice[i];

    if ((sampleData == NULL) || (sampleLength < 1))
    {
        v->sampleData8  = NULL;
        v->sampleData16 = NULL;
        return;
    }

    if (position >= sampleLength)
    {
        position = 0;
        v->sampleData8  = NULL;
        v->sampleData16 = NULL;
    }
    else
    {
        v->sampleData8  = sampleData;
        v->sampleData16 = (const int16_t *)(sampleData);
    }

    if (sixteenbit)
    {
        sampleLoopBegin  = (sampleLoopBegin  & 0xFFFFFFFE) / 2;
        sampleLength     = (sampleLength     & 0xFFFFFFFE) / 2;
        sampleLoopLength = (sampleLoopLength & 0xFFFFFFFE) / 2;
        sampleLoopEnd    = (sampleLoopEnd    & 0xFFFFFFFE) / 2;

        v->sampleData16R = &v->sampleData16[sampleLength];
    }
    else
    {
        v->sampleData8R = &v->sampleData8[sampleLength];
    }

    if (sampleLoopLength < 2) /* FT2 can do 1-sample loops, but we don't (for security reasons) */
        loopFlag = false;

    v->frac             = 0.0f;
    v->sample16bit      = sixteenbit ? true : false;
    v->loopingBackwards = false;
    v->samplePosition   = position;
    v->sampleLength     = sampleLength;
    v->sampleLoopBegin  = sampleLoopBegin;
    v->sampleLoopEnd    = sampleLoopEnd;
    v->sampleLoopLength = sampleLoopLength;;
    v->loop             = loopFlag ? ((loopFlag & 2) ? 2 : 1) : 0;
    v->stereo           = stereo ? true : false;
}

static void voiceSetVolume(uint8_t i, uint16_t vol)
{
    voice[i].volume = vol / 256.0f;
}

static void voiceSetPanning(uint8_t i, uint8_t pan)
{
    voice_t *v;

    v = &voice[i];

    v->panL = panningTab[256 - pan] / 65536.0f;
    v->panR = panningTab[      pan] / 65536.0f;
}

static void voiceSetSamplingFreq(uint8_t i, uint32_t samplingFrequency)
{
    if (f_audioFreq > 0.0f)
        voice[i].delta = samplingFrequency / f_audioFreq;
}

static void voiceTrigger(uint8_t chNr)
{
    stmTyp *ch;
    sampleTyp *s;

    ch = &stm[chNr];

    s = ch->smpPtr;
    voiceSetSource(chNr, s->pek, s->len, s->repS, s->repL, s->repS + s->repL, s->typ & 3, s->typ & 16,
                   s->typ & 32, ch->smpStartPos);
}

static void updateChannelVolPanFrq(void)
{
    uint8_t i, status;
    stmTyp *ch;

    for (i = 0; i < song.antChn; ++i)
    {
        ch = &stm[i];

        status = ch->status;
        if (status != 0)
        {
            ch->status = 0;

            /* this order is carefully selected, modification can result in unwanted behavior */
            if (status & IS_NyTon)          voiceSetVolRamp(i);
            if (status & IS_Vol)            voiceSetVolume(i, ch->finalVol);
            if (status & IS_Pan)            voiceSetPanning(i, ch->finalPan);
            if (status & (IS_Vol | IS_Pan)) voiceUpdateVolumes(i, status);
            if (status & IS_Period)         voiceSetSamplingFreq(i, getFrequenceValue(ch->finalPeriod));
            if (status & IS_NyTon)          voiceTrigger(i);
        }
    }
}

static inline void noNewAllChannels(void)
{
    uint8_t i;

    for (i = 0; i < song.antChn; ++i)
    {
        doEffects(&stm[i]);
        fixaEnvelopeVibrato(&stm[i]);
    }
}

static void getNextPos(void)
{
    if (song.timer == 1)
    {
        song.pattPos++;

        if (song.pattDelTime)
        {
            song.pattDelTime2 = song.pattDelTime;
            song.pattDelTime  = 0;
        }

        if (song.pattDelTime2)
        {
            song.pattDelTime2--;
            if (song.pattDelTime2)
                song.pattPos--;
        }

        if (song.pBreakFlag)
        {
            song.pBreakFlag = false;
            song.pattPos    = song.pBreakPos;
        }

        if ((song.pattPos >= song.pattLen) || song.posJumpFlag)
        {
            song.pattPos     = song.pBreakPos;
            song.pBreakPos   = 0;
            song.posJumpFlag = false;

            if (++song.songPos >= song.len)
                  song.songPos  = song.repS;

            song.pattNr  = song.songTab[song.songPos & 0xFF];
            song.pattLen = pattLens[song.pattNr & 0xFF];
        }
    }
}

static void mainPlayer(void) /* periodically called from mixer */
{
    uint8_t i, readNewNote;

    if (musicPaused || !songPlaying)
    {
        for (i = 0; i < song.antChn; ++i)
            fixaEnvelopeVibrato(&stm[i]);
    }
    else
    {
        readNewNote = false;
        if (--song.timer == 0)
        {
            song.timer = song.tempo;
            readNewNote = true;
        }

        if (readNewNote)
        {
            if (!song.pattDelTime2)
            {
                for (i = 0; i < song.antChn; ++i)
                {
                    if (patt[song.pattNr] != NULL)
                        getNewNote(&stm[i], &patt[song.pattNr][(song.pattPos * song.antChn) + i]);
                    else
                        getNewNote(&stm[i], &nilPatternLine);

                    fixaEnvelopeVibrato(&stm[i]);
                }
            }
            else
            {
                noNewAllChannels();
            }
        }
        else
        {
            noNewAllChannels();
        }

        getNextPos();
    }
}

static void stopVoice(uint8_t i)
{
    voice_t *v;

    v = &voice[i];
    memset(v, 0, sizeof (voice_t));

    v->sampleData8  = NULL;
    v->sampleData16 = NULL;
    v->panL = 128.0f;
    v->panR = 128.0f;

    /* clear vol ramp voice too */

    v = &voice[MAX_VOICES + i];
    memset(v, 0, sizeof (voice_t));

    v->sampleData8  = NULL;
    v->sampleData16 = NULL;
    v->panL = 128.0f;
    v->panR = 128.0f;
}

static void stopVoices(void)
{
    uint8_t i;
    stmTyp *ch;

    memset(voice, 0, sizeof (voice));
    memset(stm,   0, sizeof (stm));

    /* non-FT2 additions for safety */
    song.pattDelTime  = 0;
    song.pattDelTime2 = 0;
    song.posJumpFlag  = false;
    song.pBreakFlag   = false;
    song.pBreakPos    = 0;
    /* ---------------------------- */

    for (i = 0; i < song.antChn; ++i)
    {
        ch = &stm[i];

        ch->instrPtr = instr[0];
        ch->status   = IS_Vol;
        ch->oldPan   = 128;
        ch->outPan   = 128;
        ch->finalPan = 128;

        stopVoice(i);
    }
}

static void setPos(int16_t songPos, int16_t pattPos)
{
    if (songPos > -1)
    {
        song.songPos = songPos;
        if ((song.len > 0) && (song.songPos >= song.len))
            song.songPos = song.len - 1;

        song.pattNr = song.songTab[songPos];
        song.pattLen = pattLens[song.pattNr];
    }

    if (pattPos > -1)
    {
        song.pattPos = pattPos;
        if (song.pattPos >= song.pattLen)
            song.pattPos  = song.pattLen - 1;
    }

    song.timer = 1;
}

static void freeMusic(void)
{
    uint16_t i, j;

    for (i = 0; i < (255 + 1); ++i)
    {
        if (instr[i] != NULL)
        {
            for (j = 0; j < 32; ++j)
            {
                if (instr[i]->samp[j].pek != NULL)
                {
                    free(instr[i]->samp[j].pek);
                    instr[i]->samp[j].pek = NULL;
                }
            }

            free(instr[i]);
            instr[i] = NULL;
        }
    }

    for (i = 0; i < 256; ++i)
    {
        if (patt[i] != NULL)
        {
            free(patt[i]);
            patt[i] = NULL;
        }
    }

    memset(&song, 0, sizeof (song));

    song.len     = 1;
    song.tempo   = 6;
    song.speed   = 125;
    song.timer   = 1;
    song.antChn  = 8;
    linearFrqTab = 1;

    setSpeed(125);
    stopVoices();
    setPos(0, 0);
}

static void delta2Samp(int8_t *p, uint32_t len, uint8_t typ)
{
    int8_t *p8, news8, olds8L, olds8R;
    int16_t *p16, news16, olds16L, olds16R;
    uint32_t i;

    if (typ & 16) len /= 2; /* 16-bit */
    if (typ & 32) len /= 2; /* stereo */

    if (typ & 32)
    {
        if (typ & 16)
        {
            p16 = (int16_t *)(p);

            olds16L = 0;
            olds16R = 0;

            for (i = 0; i < len; ++i)
            {
                news16  = p16[i] + olds16L;
                p16[i]  = news16;
                olds16L = news16;

                news16 = p16[len + i] + olds16R;
                p16[len + i] = news16;
                olds16R = news16;
            }
        }
        else
        {
            p8 = (int8_t *)(p);

            olds8L = 0;
            olds8R = 0;

            for (i = 0; i < len; ++i)
            {
                news8  = p8[i] + olds8L;
                p8[i]  = news8;
                olds8L = news8;

                news8 = p8[len + i] + olds8R;
                p8[len + i] = news8;
                olds8R = news8;
            }
        }
    }
    else
    {
        if (typ & 16)
        {
            p16 = (int16_t *)(p);

            olds16L = 0;

            for (i = 0; i < len; ++i)
            {
                news16  = p16[i] + olds16L;
                p16[i]  = news16;
                olds16L = news16;
            }
        }
        else
        {
            p8 = (int8_t *)(p);

            olds8L = 0;

            for (i = 0; i < len; ++i)
            {
                news8  = p8[i] + olds8L;
                p8[i]  = news8;
                olds8L = news8;
            }
        }
    }
}

static void freeInstr(uint16_t ins)
{
    uint8_t i;

    if (instr[ins] != NULL)
    {
        for (i = 0; i < 32; ++i)
        {
            if (instr[ins]->samp[i].pek != NULL)
            {
                free(instr[ins]->samp[i].pek);
                instr[ins]->samp[i].pek = NULL;
            }
        }

        free(instr[ins]);
        instr[ins] = NULL;
    }
}

static void freeAllInstr(void)
{
    uint16_t i;

    for (i = 0; i < 256; ++i)
        freeInstr(i);
}

static int8_t allocateInstr(uint16_t i)
{
    uint8_t j;
    instrTyp *p;

    if (instr[i] == NULL)
    {
        p = (instrTyp *)(calloc(1, sizeof (instrTyp)));
        if (p == NULL) return (0);

        for (j = 0; j < 32; ++j)
        {
            p->samp[j].pan = 128;
            p->samp[j].vol = 64;
        }

        instr[i] = p;

        return (1);
    }

    return (0);
}

static int8_t loadInstrHeader(MEM *f, uint16_t i)
{
    uint8_t j;
    instrHeaderTyp ih;
    sampleTyp *s;

    memset(&ih, 0, 263);

    mread(&ih.instrSize, 4, 1, f);
    if ((ih.instrSize <= 0) || (ih.instrSize > 263))
        ih.instrSize = 263;

    mread(ih.name, ih.instrSize - 4, 1, f);

    if (meof(f) || (ih.antSamp > 32))
        return (false);

    if (ih.antSamp > 0)
    {
        if (!allocateInstr(i))
            return (0);

        memcpy(instr[i]->ta, ih.ta, ih.instrSize);
        instr[i]->antSamp = ih.antSamp;

        mread(ih.samp, ih.antSamp * sizeof (sampleHeaderTyp), 1, f);
        if (meof(f))
            return (0);

        for (j = 0; j < ih.antSamp; ++j)
        {
            s = &instr[i]->samp[j];

            memcpy(&s->len, &ih.samp[j].len, 12 + 4 + 24);
            if (s->vol > 64)
                s->vol = 64;
        }
    }

    return (1);
}

static int8_t loadInstrSample(MEM *f, uint16_t i)
{
    uint16_t j;
    int32_t l;
    sampleTyp *s;

    if (instr[i] != NULL)
    {
        for (j = 1; j <= instr[i]->antSamp; ++j)
        {
            s = &instr[i]->samp[j - 1];

            s->pek = NULL;

            l = s->len;
            if (l > 0)
            {
                s->pek = (int8_t *)(malloc(l));
                if (s->pek == NULL)
                {
                    for (j = i; j <= song.antInstrs; ++j)
                        freeInstr(j);

                    return (false);
                }

                mread(s->pek, l, 1, f);
                delta2Samp(s->pek, l, s->typ);

                if (s->typ & 32)
                    s->len /= 2;
            }

            if (s->pek == NULL)
            {
                s->len  = 0;
                s->repS = 0;
                s->repL = 0;
            }
            else
            {
                if (s->repS < 0) s->repS = 0;
                if (s->repL < 0) s->repL = 0;
                if (s->repS > s->len) s->repS = s->len;
                if ((s->repS + s->repL) > s->len) s->repL = s->len - s->repS;
            }
        }
    }

    return (true);
}

static void unpackPatt(tonTyp *patdata, uint16_t length, uint16_t packlen, uint8_t *packdata)
{
    uint8_t j, packnote;
    uint32_t i, patofs, packindex;

    packindex = 0;
    for (i = 0; i < length; ++i)
    {
        for (j = 0; j < song.antChn; ++j)
        {
            if (packindex >= packlen)
                return;

            patofs   = (i * song.antChn) + j;
            packnote = packdata[packindex++];

            if (packnote & 0x80)
            {
                if (packnote & 0x01) patdata[patofs].ton    = packdata[packindex++];
                if (packnote & 0x02) patdata[patofs].instr  = packdata[packindex++];
                if (packnote & 0x04) patdata[patofs].vol    = packdata[packindex++];
                if (packnote & 0x08) patdata[patofs].effTyp = packdata[packindex++];
                if (packnote & 0x10) patdata[patofs].eff    = packdata[packindex++];
            }
            else
            {
                patdata[patofs].ton    = packnote;
                patdata[patofs].instr  = packdata[packindex++];
                patdata[patofs].vol    = packdata[packindex++];
                patdata[patofs].effTyp = packdata[packindex++];
                patdata[patofs].eff    = packdata[packindex++];
            }
        }
    }
}

static int8_t patternEmpty(uint16_t nr)
{
    uint8_t j, *pattPtr;
    uint16_t i;

    if (patt[nr] == NULL)
    {
        return (true);
    }
    else
    {
        pattPtr = (uint8_t *)(patt[nr]);
        for (i = 0; i < pattLens[nr]; ++i)
        {
            for (j = 0; j < song.antChn; ++j)
            {
                if (*pattPtr++) return (false);
                if (*pattPtr++) return (false);
                if (*pattPtr++) return (false);
                if (*pattPtr++) return (false);
                if (*pattPtr++) return (false);
            }
        }
    }

    return (true);
}

static int8_t loadPatterns(MEM *f)
{
    uint8_t tmpLen, *patttmp;
    uint16_t i;
    patternHeaderTyp ph;

    for (i = 0; i < song.antPtn; ++i)
    {
        mread(&ph.patternHeaderSize, 4, 1, f);
        mread(&ph.typ, 1, 1, f);

        ph.pattLen = 0;
        if (song.ver == 0x0102)
        {
            mread(&tmpLen, 1, 1, f);
            ph.pattLen = (uint16_t)(tmpLen) + 1; /* +1 in v1.02 */
        }
        else
        {
            mread(&ph.pattLen, 2, 1, f);
        }

        mread(&ph.dataLen, 2, 1, f);

        if (song.ver == 0x0102)
        {
            if (ph.patternHeaderSize > 8)
                mseek(f, ph.patternHeaderSize - 8, SEEK_CUR);
        }
        else
        {
            if (ph.patternHeaderSize > 9)
                mseek(f, ph.patternHeaderSize - 9, SEEK_CUR);
        }

        if (meof(f))
        {
            mclose(&f);
            return (false);
        }

        pattLens[i] = ph.pattLen;
        if (ph.dataLen)
        {
            patt[i] = (tonTyp *)(calloc(sizeof (tonTyp), ph.pattLen * song.antChn));
            if (patt[i] == NULL)
            {
                mclose(&f);
                return (false);
            }

            patttmp = (uint8_t *)(malloc(ph.dataLen));
            if (patttmp == NULL)
            {
                mclose(&f);
                return (false);
            }

            mread(patttmp, ph.dataLen, 1, f);
            unpackPatt(patt[i], ph.pattLen, ph.dataLen, patttmp);
            free(patttmp);
        }

        if (patternEmpty(i))
        {
            if (patt[i] != NULL)
            {
                free(patt[i]);
                patt[i] = NULL;
            }

            pattLens[i] = 64;
        }
    }

    return (true);
}

int8_t ft2play_LoadModule(const uint8_t *moduleData, uint32_t dataLength)
{
    uint16_t i;
    songHeaderTyp h;
    MEM *f;

    if (moduleLoaded)
        ft2play_FreeSong();

    moduleLoaded = false;

    freeMusic();
    linearFrqTab = 0;

    f = mopen(moduleData, dataLength);
    if (f == NULL) return (0);

    /* start loading */
    mread(&h, sizeof (h), 1, f);

    if ((memcmp(h.sig, "Extended Module: ", 17) != 0) || (h.ver < 0x0102) || (h.ver > 0x104))
    {
        mclose(&f);
        return (0);
    }

    if ((h.antChn < 1) || (h.antChn > MAX_VOICES) || (h.antPtn > 256))
    {
        mclose(&f);
        return (0);
    }

    mseek(f, 60 + h.headerSize, SEEK_SET);
    if (meof(f))
    {
        mclose(&f);
        return (0);
    }

    song.len       = h.len;
    song.repS      = h.repS;
    song.antChn    = (uint8_t)(h.antChn);
    song.speed     = h.defSpeed ? h.defSpeed : 125;
    song.tempo     = h.defTempo ? h.defTempo : 6;
    song.antInstrs = h.antInstrs;
    song.antPtn    = h.antPtn;
    song.ver       = h.ver;
    linearFrqTab   = h.flags & 1;

    memcpy(song.songTab, h.songTab, h.len);

    if (song.ver < 0x0104)
    {
        for (i = 1; i <= h.antInstrs; ++i)
        {
            if (!loadInstrHeader(f, i))
            {
                freeAllInstr();
                mclose(&f);
                return (0);
            }
        }

        if (!loadPatterns(f))
        {
            freeAllInstr();
            mclose(&f);
            return (0);
        }

        for (i = 1; i <= h.antInstrs; ++i)
        {
            if (!loadInstrSample(f, i))
            {
                freeAllInstr();
                mclose(&f);
                return (0);
            }
        }
    }
    else
    {
        if (!loadPatterns(f))
        {
            mclose(&f);
            return (0);
        }

        for (i = 1; i <= h.antInstrs; ++i)
        {
            if (!loadInstrHeader(f, i))
            {
                freeInstr((uint8_t)(i));
                mclose(&f);
                break;
            }

            if (!loadInstrSample(f, i))
            {
                mclose(&f);
                break;
            }
        }
    }

    mclose(&f);

    note2Period = linearFrqTab ? linearPeriods : amigaPeriods;

    if (song.repS > song.len)
        song.repS = 0;

    stopVoices();
    setPos(0, 0);

    /* instr 0 is a placeholder for invalid instruments */
    allocateInstr(0);
    instr[0]->samp[0].vol = 0;
    /* ------------------------------------------------ */

    moduleLoaded = true;

    return (true);
}

static inline int8_t set8bitSample(voice_t *v)
{
    int32_t newPos, temp;

    newPos = v->samplePosition;
    if (v->loop == 0) /* no loop */
    {
        if (newPos >= v->sampleLength)
            return (false);
    }
    else if (v->loopingBackwards) /* definitely bidi loop */
    {
        if (newPos < v->sampleLoopBegin)
        {
            v->loopingBackwards = false;
            newPos = v->sampleLoopBegin + ((v->sampleLoopBegin - newPos - 1) % v->sampleLoopLength);
        }
    }
    else if (newPos >= v->sampleLoopEnd) /* forward or bidi loop */
    {
        temp = (newPos - v->sampleLoopEnd) % v->sampleLoopLength;

        if (v->loop == 2) /* bidi loop */
        {
            v->loopingBackwards = true;
            newPos = (v->sampleLoopEnd - 1) - temp;
        }
        else /* forward loop */
        {
            newPos = v->sampleLoopBegin + temp;
        }
    }

    v->samplePosition = newPos;
    v->sampleL = v->sampleData8[newPos] * (1.0f / 128.0f);

    if (samplingInterpolation)
    {
        /* calculate next pos for linear interpolation routine */
        newPos++;
        if (v->loop)
        {
            if (v->loopingBackwards) /* definitely bidi loop */
            {
                newPos = v->samplePosition - 1;
                if (newPos < v->sampleLoopBegin)
                    newPos = v->sampleLoopBegin;
            }
            else if (newPos >= v->sampleLoopEnd) /* forward or bidi loop */
            {
                if (v->loop == 2) /* bidi loop */
                    newPos = v->sampleLoopEnd - 1;
                else /* forward loop */
                    newPos = v->sampleLoopBegin;
            }
        }
        else if (newPos >= v->sampleLength)
        {
            v->nextSampleL = 0.0f;
            return (true);
        }

        v->nextSampleL = v->sampleData8[newPos] * (1.0f / 128.0f);
    }

    return (true);
}

static inline int8_t set16bitSample(voice_t *v)
{
    int32_t newPos, temp;

    newPos = v->samplePosition;
    if (v->loop == 0) /* no loop */
    {
        if (newPos >= v->sampleLength)
            return (false);
    }
    else if (v->loopingBackwards) /* definitely bidi loop */
    {
        if (newPos < v->sampleLoopBegin)
        {
            v->loopingBackwards = false;
            newPos = v->sampleLoopBegin + ((v->sampleLoopBegin - newPos - 1) % v->sampleLoopLength);
        }
    }
    else if (newPos >= v->sampleLoopEnd) /* forward or bidi loop */
    {
        temp = (newPos - v->sampleLoopEnd) % v->sampleLoopLength;

        if (v->loop == 2) /* bidi loop */
        {
            v->loopingBackwards = true;
            newPos = (v->sampleLoopEnd - 1) - temp;
        }
        else /* forward loop */
        {
            newPos = v->sampleLoopBegin + temp;
        }
    }

    v->samplePosition = newPos;
    v->sampleL = v->sampleData16[newPos] * (1.0f / 32768.0f);

    /* calculate next pos for linear interpolation routine */
    if (samplingInterpolation)
    {
        newPos++;
        if (v->loop)
        {
            if (v->loopingBackwards) /* definitely bidi loop */
            {
                newPos = v->samplePosition - 1;
                if (newPos < v->sampleLoopBegin)
                    newPos = v->sampleLoopBegin;
            }
            else if (newPos >= v->sampleLoopEnd) /* forward or bidi loop */
            {
                if (v->loop == 2) /* bidi loop */
                    newPos = v->sampleLoopEnd - 1;
                else /* forward loop */
                    newPos = v->sampleLoopBegin;
            }
        }
        else if (newPos >= v->sampleLength)
        {
            v->nextSampleL = 0.0f;
            return (true);
        }

        v->nextSampleL = v->sampleData16[newPos] * (1.0f / 32768.0f);
    }

    return (true);
}

static inline int8_t set8bitStereoSample(voice_t *v)
{
    int32_t newPos, temp;

    newPos = v->samplePosition;
    if (v->loop == 0) /* no loop */
    {
        if (newPos >= v->sampleLength)
            return (false);
    }
    else if (v->loopingBackwards) /* definitely bidi loop */
    {
        if (newPos < v->sampleLoopBegin)
        {
            v->loopingBackwards = false;
            newPos = v->sampleLoopBegin + ((v->sampleLoopBegin - newPos - 1) % v->sampleLoopLength);
        }
    }
    else if (newPos >= v->sampleLoopEnd) /* forward or bidi loop */
    {
        temp = (newPos - v->sampleLoopEnd) % v->sampleLoopLength;

        if (v->loop == 2) /* bidi loop */
        {
            v->loopingBackwards = true;
            newPos = (v->sampleLoopEnd - 1) - temp;
        }
        else /* forward loop */
        {
            newPos = v->sampleLoopBegin + temp;
        }
    }

    v->samplePosition = newPos;

    v->sampleL = v->sampleData8[newPos]  * (1.0f / 128.0f);
    v->sampleR = v->sampleData8R[newPos] * (1.0f / 128.0f);

    if (samplingInterpolation)
    {
        /* calculate next pos for linear interpolation routine */
        newPos++;
        if (v->loop)
        {
            if (v->loopingBackwards) /* definitely bidi loop */
            {
                newPos = v->samplePosition - 1;
                if (newPos < v->sampleLoopBegin)
                    newPos = v->sampleLoopBegin;
            }
            else if (newPos >= v->sampleLoopEnd) /* forward or bidi loop */
            {
                if (v->loop == 2) /* bidi loop */
                    newPos = v->sampleLoopEnd - 1;
                else /* forward loop */
                    newPos = v->sampleLoopBegin;
            }
        }
        else if (newPos >= v->sampleLength)
        {
            v->nextSampleL = 0.0f;
            v->nextSampleR = 0.0f;

            return (true);
        }

        v->nextSampleL = v->sampleData8[newPos]  * (1.0f / 128.0f);
        v->nextSampleR = v->sampleData8R[newPos] * (1.0f / 128.0f);
    }

    return (true);
}

static inline int8_t set16bitStereoSample(voice_t *v)
{
    int32_t newPos, temp;

    newPos = v->samplePosition;
    if (v->loop == 0) /* no loop */
    {
        if (newPos >= v->sampleLength)
            return (false);
    }
    else if (v->loopingBackwards) /* definitely bidi loop */
    {
        if (newPos < v->sampleLoopBegin)
        {
            v->loopingBackwards = false;
            newPos = v->sampleLoopBegin + ((v->sampleLoopBegin - newPos - 1) % v->sampleLoopLength);
        }
    }
    else if (newPos >= v->sampleLoopEnd) /* forward or bidi loop */
    {
        temp = (newPos - v->sampleLoopEnd) % v->sampleLoopLength;

        if (v->loop == 2) /* bidi loop */
        {
            v->loopingBackwards = true;
            newPos = (v->sampleLoopEnd - 1) - temp;
        }
        else /* forward loop */
        {
            newPos = v->sampleLoopBegin + temp;
        }
    }

    v->samplePosition = newPos;

    v->sampleL = v->sampleData16[newPos]  * (1.0f / 32768.0f);
    v->sampleR = v->sampleData16R[newPos] * (1.0f / 32768.0f);

    /* calculate next pos for linear interpolation routine */
    if (samplingInterpolation)
    {
        newPos++;
        if (v->loop)
        {
            if (v->loopingBackwards) /* definitely bidi loop */
            {
                newPos = v->samplePosition - 1;
                if (newPos < v->sampleLoopBegin)
                    newPos = v->sampleLoopBegin;
            }
            else if (newPos >= v->sampleLoopEnd) /* forward or bidi loop */
            {
                if (v->loop == 2) /* bidi loop */
                    newPos = v->sampleLoopEnd - 1;
                else /* forward loop */
                    newPos = v->sampleLoopBegin;
            }
        }
        else if (newPos >= v->sampleLength)
        {
            v->nextSampleL = 0.0f;
            v->nextSampleR = 0.0f;

            return (true);
        }

        v->nextSampleL = v->sampleData16[newPos]  * (1.0f / 32768.0f);
        v->nextSampleR = v->sampleData16R[newPos] * (1.0f / 32768.0f);
    }

    return (true);
}

static void mix8b(voice_t *v, int32_t samples)
{
    int32_t i, intFrac;
    float sample;

    for (i = 0; (v->sampleData8 != NULL) && (i < samples); ++i)
    {
        if (v->setNewSmp)
        {
            v->setNewSmp = false;
            if (!set8bitSample(v))
            {
                v->sampleData8 = NULL;
                break;
            }
        }

        if (samplingInterpolation)
            sample = LERP(v->sampleL, v->nextSampleL, v->frac);
        else
            sample = v->sampleL;

        if (volumeRamping)
        {
            v->fader += v->faderDelta;

            if ((v->faderDelta > 0.0f) && (v->fader > v->faderDest))
            {
                v->fader = v->faderDest;
            }
            else if ((v->faderDelta < 0.0f) && (v->fader < v->faderDest))
            {
                v->fader = v->faderDest;
                v->sampleData8 = NULL;
                break;
            }

            sample *= v->fader;
        }

        mixerBufferL[i] += (sample * v->volumeL);
        mixerBufferR[i] += (sample * v->volumeR);

        if (volumeRamping)
        {
            v->volumeL += v->volDeltaL;
            v->volumeR += v->volDeltaR;

                 if ((v->volDeltaL > 0.0f) && (v->volumeL > v->targetVolL)) v->volumeL = v->targetVolL;
            else if ((v->volDeltaL < 0.0f) && (v->volumeL < v->targetVolL)) v->volumeL = v->targetVolL;
                 if ((v->volDeltaR > 0.0f) && (v->volumeR > v->targetVolR)) v->volumeR = v->targetVolR;
            else if ((v->volDeltaR < 0.0f) && (v->volumeR < v->targetVolR)) v->volumeR = v->targetVolR;
        }

        v->frac += v->delta;
        if (v->frac >= 1.0f)
        {
            intFrac = (int32_t)(v->frac);
            v->frac -= intFrac;

            if (v->loopingBackwards)
                v->samplePosition -= intFrac;
            else
                v->samplePosition += intFrac;

            v->setNewSmp = true;
        }
    }
}

static void mix8bstereo(voice_t *v, int32_t samples)
{
    int32_t i, intFrac;
    float sampleL, sampleR;

    for (i = 0; (v->sampleData8 != NULL) && (i < samples); ++i)
    {
        if (v->setNewSmp)
        {
            v->setNewSmp = false;
            if (!set8bitStereoSample(v))
            {
                v->sampleData8 = NULL;
                break;
            }
        }

        if (samplingInterpolation)
        {
            sampleL = LERP(v->sampleL, v->nextSampleL, v->frac);
            sampleR = LERP(v->sampleR, v->nextSampleR, v->frac);
        }
        else
        {
            sampleL = v->sampleL;
            sampleR = v->sampleR;
        }

        if (volumeRamping)
        {
            v->fader += v->faderDelta;

            if ((v->faderDelta > 0.0f) && (v->fader > v->faderDest))
            {
                v->fader = v->faderDest;
            }
            else if ((v->faderDelta < 0.0f) && (v->fader < v->faderDest))
            {
                v->fader = v->faderDest;
                v->sampleData8 = NULL;
                break;
            }

            sampleL *= v->fader;
            sampleR *= v->fader;
        }

        mixerBufferL[i] += (sampleL * v->volumeL);
        mixerBufferR[i] += (sampleR * v->volumeR);

        if (volumeRamping)
        {
            v->volumeL += v->volDeltaL;
            v->volumeR += v->volDeltaR;

                 if ((v->volDeltaL > 0.0f) && (v->volumeL > v->targetVolL)) v->volumeL = v->targetVolL;
            else if ((v->volDeltaL < 0.0f) && (v->volumeL < v->targetVolL)) v->volumeL = v->targetVolL;
                 if ((v->volDeltaR > 0.0f) && (v->volumeR > v->targetVolR)) v->volumeR = v->targetVolR;
            else if ((v->volDeltaR < 0.0f) && (v->volumeR < v->targetVolR)) v->volumeR = v->targetVolR;
        }

        v->frac += v->delta;
        if (v->frac >= 1.0f)
        {
            intFrac = (int32_t)(v->frac);
            v->frac -= intFrac;

            if (v->loopingBackwards)
                v->samplePosition -= intFrac;
            else
                v->samplePosition += intFrac;

            v->setNewSmp = true;
        }
    }
}

static void mix16b(voice_t *v, int32_t samples)
{
    int32_t i, intFrac;
    float sample;

    for (i = 0; (v->sampleData16 != NULL) && (i < samples); ++i)
    {
        if (v->setNewSmp)
        {
            v->setNewSmp = false;
            if (!set16bitSample(v))
            {
                v->sampleData16 = NULL;
                break;
            }
        }

        if (samplingInterpolation)
            sample = LERP(v->sampleL, v->nextSampleL, v->frac);
        else
            sample = v->sampleL;

        if (volumeRamping)
        {
            v->fader += v->faderDelta;

            if ((v->faderDelta > 0.0f) && (v->fader > v->faderDest))
            {
                v->fader = v->faderDest;
            }
            else if ((v->faderDelta < 0.0f) && (v->fader < v->faderDest))
            {
                v->fader = v->faderDest;
                v->sampleData16 = NULL;
                break;
            }

            sample *= v->fader;
        }

        mixerBufferL[i] += (sample * v->volumeL);
        mixerBufferR[i] += (sample * v->volumeR);

        if (volumeRamping)
        {
            v->volumeL += v->volDeltaL;
            v->volumeR += v->volDeltaR;

                 if ((v->volDeltaL > 0.0f) && (v->volumeL > v->targetVolL)) v->volumeL = v->targetVolL;
            else if ((v->volDeltaL < 0.0f) && (v->volumeL < v->targetVolL)) v->volumeL = v->targetVolL;
                 if ((v->volDeltaR > 0.0f) && (v->volumeR > v->targetVolR)) v->volumeR = v->targetVolR;
            else if ((v->volDeltaR < 0.0f) && (v->volumeR < v->targetVolR)) v->volumeR = v->targetVolR;
        }

        v->frac += v->delta;
        if (v->frac >= 1.0f)
        {
            intFrac = (int32_t)(v->frac);
            v->frac -= intFrac;

            if (v->loopingBackwards)
                v->samplePosition -= intFrac;
            else
                v->samplePosition += intFrac;

            v->setNewSmp = true;
        }
    }
}

static void mix16bstereo(voice_t *v, int32_t samples)
{
    int32_t i, intFrac;
    float sampleL, sampleR;

    for (i = 0; (v->sampleData16 != NULL) && (i < samples); ++i)
    {
        if (v->setNewSmp)
        {
            v->setNewSmp = false;
            if (!set16bitStereoSample(v))
            {
                v->sampleData16 = NULL;
                break;
            }
        }

        if (samplingInterpolation)
        {
            sampleL = LERP(v->sampleL, v->nextSampleL, v->frac);
            sampleR = LERP(v->sampleR, v->nextSampleR, v->frac);
        }
        else
        {
            sampleL = v->sampleL;
            sampleR = v->sampleR;
        }

        if (volumeRamping)
        {
            v->fader += v->faderDelta;

            if ((v->faderDelta > 0.0f) && (v->fader > v->faderDest))
            {
                v->fader = v->faderDest;
            }
            else if ((v->faderDelta < 0.0f) && (v->fader < v->faderDest))
            {
                v->fader = v->faderDest;
                v->sampleData16 = NULL;
                break;
            }

            sampleL *= v->fader;
            sampleR *= v->fader;
        }

        mixerBufferL[i] += (sampleL * v->volumeL);
        mixerBufferR[i] += (sampleR * v->volumeR);

        if (volumeRamping)
        {
            v->volumeL += v->volDeltaL;
            v->volumeR += v->volDeltaR;

                 if ((v->volDeltaL > 0.0f) && (v->volumeL > v->targetVolL)) v->volumeL = v->targetVolL;
            else if ((v->volDeltaL < 0.0f) && (v->volumeL < v->targetVolL)) v->volumeL = v->targetVolL;
                 if ((v->volDeltaR > 0.0f) && (v->volumeR > v->targetVolR)) v->volumeR = v->targetVolR;
            else if ((v->volDeltaR < 0.0f) && (v->volumeR < v->targetVolR)) v->volumeR = v->targetVolR;
        }

        v->frac += v->delta;
        if (v->frac >= 1.0f)
        {
            intFrac = (int32_t)(v->frac);
            v->frac -= intFrac;

            if (v->loopingBackwards)
                v->samplePosition -= intFrac;
            else
                v->samplePosition += intFrac;

            v->setNewSmp = true;
        }
    }
}

static void mixChannels(uint32_t samples)
{
    int32_t i;
    voice_t *v;

    for (i = 0; i < song.antChn; ++i)
    {
        v = &voice[i];

        if (v->stereo)
        {
            if (v->sample16bit)
                mix16bstereo(v, samples);
            else
                mix8bstereo(v, samples);

            v = &voice[MAX_VOICES + i];
            if (v->sample16bit)
                mix16bstereo(v, samples);
            else
                mix8bstereo(v, samples);
        }
        else
        {
            if (v->sample16bit)
                mix16b(v, samples);
            else
                mix8b(v, samples);

            v = &voice[MAX_VOICES + i];
            if (v->sample16bit)
                mix16b(v, samples);
            else
                mix8b(v, samples);
        }
    }
}

static void mixAudio(int16_t *outputStream, int32_t sampleBlockLength)
{
    int16_t *streamPointer;
    int32_t i, out32;
    float out_f;

    memset(mixerBufferL, 0, sampleBlockLength * sizeof (float));
    memset(mixerBufferR, 0, sampleBlockLength * sizeof (float));

    mixChannels(sampleBlockLength);

    streamPointer = outputStream;
    for (i = 0; i < sampleBlockLength; ++i)
    {
        if (musicPaused)
        {
            *streamPointer++ = 0;
            *streamPointer++ = 0;
        }
        else
        {
            out_f = mixerBufferL[i] * (32768.0f / 3.0f);
            out32 = (int32_t)(out_f);
            CLAMP16(out32);
            *streamPointer++ = (int16_t)(out32);

            out_f = mixerBufferR[i] * (32768.0f / 3.0f);
            out32 = (int32_t)(out_f);
            CLAMP16(out32);
            *streamPointer++ = (int16_t)(out32);
        }
    }
}

void ft2play_FillAudioBuffer(int16_t *buffer, int32_t samples)
{
    int32_t a, b;

    a = samples;
    while (a > 0)
    {
        if (pmpLeft <= 0)
        {
            mainPlayer();
            updateChannelVolPanFrq();

            pmpLeft = speedVal; /* fetch speedVal after running tick */
        }

        b = a;
        if (b > pmpLeft)
            b = pmpLeft;

        mixAudio(buffer, b);
        buffer += (b * 2);

        a -= b;
        pmpLeft -= b;
    }
}

/* the following must be changed if you want to use another audio API than WinMM */

#define MIX_BUF_NUM 4

static int8_t *winmmBuffer = NULL;
static WAVEHDR waveBlocks[MIX_BUF_NUM];
static HWAVEOUT hWaveOut;
static WAVEFORMATEX wfx;

static void CALLBACK callbackWinMM(HWAVEOUT _hWaveOut, UINT uMsg,
    DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    WAVEHDR *waveBlockHeader;

    /* make compiler happy! */
    (void)(dwParam2);
    (void)(dwInstance);

    if (uMsg == MM_WOM_DONE)
    {
        mixingMutex = true;

        waveBlockHeader = (WAVEHDR *)(dwParam1);
        waveOutUnprepareHeader(_hWaveOut, waveBlockHeader, sizeof (WAVEHDR));

        if (isMixing)
        {
            memcpy(waveBlockHeader->lpData, winmmBuffer, soundBufferSize);
            waveOutPrepareHeader(_hWaveOut, waveBlockHeader, sizeof (WAVEHDR));
            waveOutWrite(_hWaveOut, waveBlockHeader, sizeof (WAVEHDR));

            ft2play_FillAudioBuffer((int16_t *)(winmmBuffer), soundBufferSize / 4);
        }

        mixingMutex = false;
    }
}

static int8_t openMixer(uint32_t _outputFrequency, uint32_t _soundBufferSize)
{
    uint32_t i;
    MMRESULT r;

    if (!hWaveOut)
    {
        winmmBuffer = (int8_t *)(calloc(_soundBufferSize, 1));
        if (winmmBuffer == NULL)
            return (false); /* gets free'd later */

        wfx.nSamplesPerSec  = _outputFrequency;
        wfx.wBitsPerSample  = 16;
        wfx.nChannels       = 2;
        wfx.wFormatTag      = WAVE_FORMAT_PCM;
        wfx.nBlockAlign     = (wfx.wBitsPerSample * wfx.nChannels) / 8;
        wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

        r = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, (DWORD_PTR)(callbackWinMM), 0L, CALLBACK_FUNCTION);
        if (r != MMSYSERR_NOERROR)
            return (false);

        for (i = 0; i < MIX_BUF_NUM; ++i)
        { 
            waveBlocks[i].dwBufferLength = _soundBufferSize;

            waveBlocks[i].lpData = (LPSTR)(calloc(_soundBufferSize, 1));
            if (waveBlocks[i].lpData == NULL)
                return (false); /* gets free'd later */

            waveOutPrepareHeader(hWaveOut, &waveBlocks[i], sizeof (WAVEHDR));
            waveOutWrite(hWaveOut, &waveBlocks[i], sizeof (WAVEHDR));
        }

        return (true);
    }

    return (true);
}

void closeMixer(void)
{
    uint32_t i;

    if (hWaveOut)
    {
        for (i = 0; i < MIX_BUF_NUM; ++i)
            waveOutUnprepareHeader(hWaveOut, &waveBlocks[i], sizeof (WAVEHDR));

        for (i = 0; i < MIX_BUF_NUM; ++i)
        {
            while (waveBlocks[i].dwFlags & WHDR_PREPARED); SleepEx(1, 1); // wait

            if (waveBlocks[i].lpData != NULL)
            {
                free(waveBlocks[i].lpData);
                waveBlocks[i].lpData = NULL;
            }
        }

        waveOutReset(hWaveOut);
        waveOutClose(hWaveOut);

        hWaveOut = 0;

        if (winmmBuffer != NULL)
        {
            free(winmmBuffer);
            winmmBuffer = NULL;
        }
    }
}

/* --------------------------------------------------------------------------- */

void ft2play_Close(void)
{
    songPlaying = false;
    musicPaused = true;

    if (isMixing)
    {
        isMixing = false;
        while (mixingMutex);
        closeMixer();
    }

    if (mixerBufferL != NULL)
    {
        free(mixerBufferL);
        mixerBufferL = NULL;
    }

    if (mixerBufferR != NULL)
    {
        free(mixerBufferR);
        mixerBufferR = NULL;
    }

    if (logTab != NULL)
    {
        free(logTab);
        logTab = NULL;
    }

    if (vibSineTab != NULL)
    {
        free(vibSineTab);
        vibSineTab = NULL;
    }

    if (amigaPeriods != NULL)
    {
        free(amigaPeriods);
        amigaPeriods = NULL;
    }

    if (linearPeriods != NULL)
    {
        free(linearPeriods);
        linearPeriods = NULL;
    }
}

int8_t ft2play_Init(uint32_t outputFreq, int8_t lerpMixFlag, int8_t volRampFlag)
{
    uint8_t j;
    int16_t noteVal;
    uint16_t i, noteIndex;

    mixerBufferL = (float *)(malloc(AUDIO_BUF_LEN * sizeof (float)));
    mixerBufferR = (float *)(malloc(AUDIO_BUF_LEN * sizeof (float)));

    if ((mixerBufferL == NULL) || (mixerBufferR == NULL))
    {
        ft2play_Close();
        return (false);
    }

    if (!openMixer(outputFreq, AUDIO_BUF_LEN))
    {
        ft2play_Close();
        return (false);
    }

    /* allocate memory for pointers */

    if (linearPeriods == NULL)
        linearPeriods = (int16_t *)(malloc(sizeof (int16_t) * ((12 * 10 * 16) + 16)));

    if (amigaPeriods == NULL)
        amigaPeriods = (int16_t *)(malloc(sizeof (int16_t) * ((12 * 10 * 16) + 16)));

    if (vibSineTab == NULL)
        vibSineTab = (int8_t *)(malloc(256));

    if (logTab == NULL)
        logTab = (uint32_t *)(malloc(sizeof (uint32_t) * 768));

    if ((linearPeriods == NULL) || (amigaPeriods == NULL) ||
        (vibSineTab    == NULL) || (logTab       == NULL))
    {
        ft2play_Close();
        return (false);
    }

    /* generate tables */

    /* generate log table (value-exact to FT2's table) */
    for (i = 0; i < (4 * 12 * 16); ++i)
        logTab[i] = (uint32_t)(((256.0 * 8363.0) * exp((i / 768.0) * log(2.0))) + 0.5);

    /* generate linear table (value-exact to FT2's table) */
    for (i = 0; i < ((12 * 10 * 16) + 16); ++i)
        linearPeriods[i] = (((12 * 10 * 16) + 16) * 4) - (i * 4);

    /* generate amiga period table (value-exact to FT2's table, except for last 17 entries) */
    noteIndex = 0;
    for (i = 0; i < 10; ++i)
    {
        for (j = 0; j < ((i == 9) ? (96 + 8) : 96); ++j)
        {
            noteVal = ((amigaFinePeriod[j % 96] * 64) + (-1 + (1 << i))) >> (i + 1);
            /* NON-FT2: j % 96. added for safety. we're patching the values later anyways. */

            amigaPeriods[noteIndex++] = noteVal;
            amigaPeriods[noteIndex++] = noteVal;
        }
    }

    /* interpolate between points (end-result is exact to FT2's table, except for last 17 entries) */
    for (i = 0; i < (12 * 10 * 8) + 7; ++i)
        amigaPeriods[(i * 2) + 1] = (amigaPeriods[i * 2] + amigaPeriods[(i * 2) + 2]) / 2;

    /*
    ** the amiga linear period table has its 17 last entries generated wrongly.
    ** the content seem to be garbage because of an 'out of boundaries' read from AmigaFinePeriods.
    ** these 17 values were taken from a memdump of FT2 in DOSBox.
    ** they might change depending on what you ran before FT2, but let's not make it too complicated.
    */

    amigaPeriods[1919] = 22; amigaPeriods[1920] = 16; amigaPeriods[1921] =  8; amigaPeriods[1922] =  0;
    amigaPeriods[1923] = 16; amigaPeriods[1924] = 32; amigaPeriods[1925] = 24; amigaPeriods[1926] = 16;
    amigaPeriods[1927] =  8; amigaPeriods[1928] =  0; amigaPeriods[1929] = 16; amigaPeriods[1930] = 32;
    amigaPeriods[1931] = 24; amigaPeriods[1932] = 16; amigaPeriods[1933] =  8; amigaPeriods[1934] =  0;
    amigaPeriods[1935] =  0;

    /* generate auto-vibrato table (value-exact to FT2's table) */
    for (i = 0; i < 256; ++i)
        vibSineTab[i] = (int8_t)(floor((64.0 * sin((-i * (2.0 * M_PI)) / 256.0)) + 0.5));

    realReplayRate        = outputFreq;
    f_audioFreq           = (float)(realReplayRate);
    soundBufferSize       = AUDIO_BUF_LEN;
    samplingInterpolation = lerpMixFlag;
    volumeRamping         = volRampFlag;

    setSpeed(125);
    quickVolRampMul_f = 1.0f / (realReplayRate / 200); /* yes, truncate the /200 */

    isMixing = true;
    return (true);
}

void ft2play_FreeSong(void)
{
    songPlaying = false;
    musicPaused = true;

    memset(voice, 0, sizeof (voice));
    while (mixingMutex);
    freeMusic();

    moduleLoaded = false;
}

void ft2play_PauseSong(int8_t pause)
{
    musicPaused = pause;
}

void ft2play_PlaySong(void)
{
    if (!moduleLoaded)
        return;

    stopVoices();

    song.globVol = 64;

    if (song.speed == 0)
        song.speed = 125;

    setSpeed(song.speed);
    setPos(0, 0);

    musicPaused = false;
    songPlaying = true;
    isMixing    = true;
}

static MEM *mopen(const uint8_t *src, uint32_t length)
{
    MEM *b;
    if ((src == NULL) || (length == 0)) return (NULL);

    b = (MEM *)(malloc(sizeof (MEM)));
    if (b == NULL) return (NULL);

    b->_base   = (uint8_t *)(src);
    b->_ptr    = (uint8_t *)(src);
    b->_cnt    = length;
    b->_bufsiz = length;
    b->_eof    = false;
 
    return (b);
}

static void mclose(MEM **buf)
{
    if (*buf != NULL)
    {
        free(*buf);
        *buf = NULL;
    }
}

static size_t mread(void *buffer, size_t size, size_t count, MEM *buf)
{
    size_t wrcnt;
    int32_t pcnt;

    if (buf       == NULL) return (0);
    if (buf->_ptr == NULL) return (0);

    wrcnt = size * count;
    if ((size == 0) || buf->_eof) return (0);

    pcnt = ((uint32_t)(buf->_cnt) > wrcnt) ? wrcnt : buf->_cnt;
    memcpy(buffer, buf->_ptr, pcnt);

    buf->_cnt -= pcnt;
    buf->_ptr += pcnt;

    if (buf->_cnt <= 0)
    {
        buf->_ptr = buf->_base + buf->_bufsiz;
        buf->_cnt = 0;
        buf->_eof = true;
    }

    return (pcnt / size);
}

static int32_t meof(MEM *buf)
{
    if (buf == NULL) return (true);
    return (buf->_eof);
}

static void mseek(MEM *buf, int32_t offset, int32_t whence)
{
    if (buf == NULL) return;

    if (buf->_base)
    {
        switch (whence)
        {
            case SEEK_SET: buf->_ptr  = buf->_base + offset;                break;
            case SEEK_CUR: buf->_ptr += offset;                             break;
            case SEEK_END: buf->_ptr  = buf->_base + buf->_bufsiz + offset; break;
            default: break;
        }

        buf->_eof = false;
        if (buf->_ptr >= (buf->_base + buf->_bufsiz))
        {
            buf->_ptr = buf->_base + buf->_bufsiz;
            buf->_eof = true;
        }

        buf->_cnt = (buf->_base + buf->_bufsiz) - buf->_ptr;
    }
}

/* EOF */