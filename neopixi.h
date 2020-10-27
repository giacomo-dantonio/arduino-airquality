#ifndef _NEOPIXI
#define _NEOPIXI

class NeoPixi
{
    public:
        NeoPixi(int nLeds) : m_nLeds(nLeds) {}

        bool update(int co2Value, int tvocValue);
        void getLedHSVColor(int index, int & hue, int & sat, int & val);

        int getNLeds() { return m_nLeds; }
        int getActivePixel() { return m_activeLed; }
        double getSeverity() { return m_severity; }

    private:
        int m_nLeds;

        int m_activeLed = -1;
        double m_activeLedIntensity;

        double m_severity;

        static constexpr int m_saturation = 255;
        static constexpr int m_value = 255;
};

#endif
