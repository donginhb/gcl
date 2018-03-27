#ifndef RTDB_LOGFILE_H
#define RTDB_LOGFILE_H

#include <ccxx/ccxx.h>
#include <vdi/measure_sharememory_all.h>

class MeasureChangedFile
{
public:
    FILE* file;
    int newCount;

};

class RtdbLogFile
{
public:
    static int
    start();

    static int
    stop();

    static std::string
    getMeasureValue(const int& value);

    static std::string
    getMeasureValue(const double& value);

    static std::string
    getMeasureValue(const StrawValue& value);

    static MeasureChangedFile*
    getMeaureChangedFile(int iMeasureId);

    static int
    checkMeaureChangedFiles();

    static std::string
    getMeasureChanngedText(int iMeasureId,
                           const int& value,
                           int iQuality,
                           msepoch_t dtChangedTime,
                           int iChangedSourceId,
                           int iChangedReasonId);

    static std::string
    getMeasureChanngedText(int iMeasureId,
                           const double& value,
                           int iQuality,
                           msepoch_t dtChangedTime,
                           int iChangedSourceId,
                           int iChangedReasonId);

    static std::string
    getMeasureChanngedText(int iMeasureId,
                           const StrawValue& value,
                           int iQuality,
                           msepoch_t dtChangedTime,
                           int iChangedSourceId,
                           int iChangedReasonId);

    template<typename T>
    static void
    insertChangeds_impl(const typename MeasureShareMemory<T>::ChangedData* const oChangedBegin,
                        const typename MeasureShareMemory<T>::ChangedData* const oChangedEnd)
    {
        const typename MeasureShareMemory<T>::ChangedData* oChangedData = oChangedBegin;
        std::vector<std::string> sSqls;
        FILE* oFile = NULL;
        std::string sText;
        size_t iWrote;
        while (oChangedData < oChangedEnd)
        {
            MeasureChangedFile* oMeasureChangedFile = getMeaureChangedFile(oChangedData->measureId);
            if (oMeasureChangedFile != NULL)
            {
                oFile = oMeasureChangedFile->file;
                if (oFile)
                {
                    sText = getMeasureChanngedText(oChangedData->measureId, oChangedData->value,
                                                   oChangedData->quality, oChangedData->changedTime,
                                                   oChangedData->changedSourceId, oChangedData->changedReasonId);
                    iWrote = fwrite(const_cast<char*>(sText.data()), 1, sText.size(), oFile);
                    if (iWrote > 0)
                    {
                        oMeasureChangedFile->newCount = oMeasureChangedFile->newCount + 1;
                    }
                    else
                    {
                        cxLogDebug() << "insertChangeds_impl error - sText: " << sText;
                    }
                }
                else
                {
                    cxLogDebug() << "insertChangeds_impl error : inter error 1.";
                }
            }
            else
            {
                cxLogDebug() << "insertChangeds_impl error : inter error 2.";
            }
            oChangedData++;
        }
    }

    static void
    insertChangeds_monsb(const MonsbShareMemory::ChangedData* const oChangedBegin,
                         const typename MonsbShareMemory::ChangedData* const oChangedEnd)
    {
        insertChangeds_impl<int>(oChangedBegin, oChangedEnd);
    }

    static void
    insertChangeds_ycadd(const YcaddShareMemory::ChangedData* const oChangedBegin,
                         const typename YcaddShareMemory::ChangedData* const oChangedEnd)
    {
        insertChangeds_impl<double>(oChangedBegin, oChangedEnd);
    }

    static void
    insertChangeds_straw(const StrawShareMemory::ChangedData* const oChangedBegin,
                         const typename StrawShareMemory::ChangedData* const oChangedEnd)
    {
        insertChangeds_impl<StrawValue>(oChangedBegin, oChangedEnd);
    }

    static size_t
    loadMeasureChangedIndex(const std::string& sFileName);

    static size_t
    loadMonsbChangedIndex();

    static size_t
    loadYcaddChangedIndex();

    static size_t
    loadStrawChangedIndex();

    static int
    saveMeasureChangedIndex(size_t iOldIndex, size_t iNewIndex, FILE* oFile);

    static int
    saveMonsbChangedIndex(size_t iOldIndex, size_t iNewIndex);

    static int
    saveYcaddChangedIndex(size_t iOldIndex, size_t iNewIndex);

    static int
    saveStrawChangedIndex(size_t iOldIndex, size_t iNewIndex);

};

#endif // RTDB_LOGFILE_H