#ifndef SEGMENTATIONPARAMETERS_H
#define SEGMENTATIONPARAMETERS_H

#include <cstdio>
#include <ctime>
#include <cstring>
#include <iostream>
#include <string>
#include <map>

class SegmentationParameters
{
    public:
        SegmentationParameters();
        SegmentationParameters(char *);
        virtual ~SegmentationParameters();

        void PrintParameters(FILE * = stdout);

        inline
        std::ostream & GetLogOut() { return m_log; }
        inline
        void SetLogOut(std::ostream & out) { m_log.rdbuf( out.rdbuf() ); }

        SegmentationParameters(const SegmentationParameters& other);
        SegmentationParameters& operator=(const SegmentationParameters& other);

        friend std::ostream & operator << (std::ostream &, const SegmentationParameters &);

        inline
        double GetFieldWidth() { return m_FieldWidth; }
        inline
        void SetFieldWidth(double val) { m_FieldWidth = val; }
        inline
        double GetFieldHeight() { return m_FieldHeight; }
        inline
        void SetFieldHeight(double val) { m_FieldHeight = val; }
        inline
        char * GetInitialFile() { return m_InitialFile; }
        inline
        void SetInitialFile(char * val) { m_InitialFile = (val) ? strdup(val) : NULL; }
        inline
        char * GetOutputDirectory() { return m_OutputDiectory; }
        inline
        void SetOutputDirectory(const char * val) { m_OutputDiectory = (val) ? strdup(val) : NULL; }
        inline
        char * GetOutputFile() { return m_OutputFile; }
        inline
        void SetOutputFile(const char * val) { m_OutputFile = (val) ? strdup(val) : NULL; }
        inline
        char * GetOutputPrefix() { return m_OutputPrefix; }
        inline
        void SetOutputPrefix(const char * val) { m_OutputPrefix = (val) ? strdup(val) : NULL; }
        inline
        time_t GetSeed() { return m_Seed; }
        inline
        void SetSeed(time_t val) { m_Seed = val; }
        inline
        char * GetPictureFile() { return m_PictureFile; }
        inline
        void SetPictureFile(char * val) { m_PictureFile = strdup(val); }
        inline
        long GetIterationsNumber() { return m_IterationsNumber; }
        inline
        void SetIterationsNumber(long val) { m_IterationsNumber = val; }
        inline
        double GetPMRRate() { return m_PMRRate; }
        inline
        void SetPMRRate(double val) { m_PMRRate = val; }

        // for debuging :
        static int _trigger;

    protected:
        std::map<std::string, std::string>  m_data;


    private:
        void OnInit();
        void LoadConfigurationFile(char *);

        double m_FieldWidth;
        double m_FieldHeight;
        char * m_InitialFile;
        char * m_OutputDiectory;
        char * m_OutputFile;
        char * m_OutputPrefix;
        char * m_PictureFile;
        time_t m_Seed;
        long m_IterationsNumber;
        double m_PMRRate;

        std::ostream & m_log;
};

#endif // SEGMENTATIONPARAMETERS_H
