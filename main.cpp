#include "mbed.h"

#define NUM_SINE_POINTS     12500  
#define SINE_LIMIT          1250
#define USEC_PER_SEC        1000000
#define LINE_FREQ_HZ        60
#define SINE_MAG            2048
#define ADC_VOLTAGE_MAG     ((float)3.3)
#define ADC_SINE_MAG        65536
#define MIN_DET_VAL         ((float)0.000001)
#define RMS_ERROR_VAL       ((float)-1.2340)


DigitalOut LTM_8522HR_DATAENABLE( D12 );
DigitalOut LTM_8522HR_DATA( D11 );
DigitalOut LTM_8522HR_CLOCK( D10 );
DigitalOut button_led( D6 );
DigitalIn  button( D9 );
DigitalOut led(LED1);
DigitalOut relay( D2 );
AnalogIn adc_in( A3 );
AnalogIn adc_ref( A4 ); 
AnalogIn thresh( A5 );

const int16_t sine_60hz[1251] = {
0,7,15,23,30,38,46,54,61,69,77,84,92,100,108,115,123,131,138,146,154,161,169,177,185,192,200,208,215,223,231,238,246,254,261,269,277,284,292,300,307,315,322,330,338,345,353,360,368,376,383,391,398,406,414,421,429,436,
444,451,459,466,474,481,489,496,504,511,519,526,534,541,549,556,563,571,578,586,593,600,608,615,623,630,637,645,652,659,667,674,681,688,696,703,710,717,725,732,739,746,753,761,768,775,782,789,796,803,810,818,825,832,839,
846,853,860,867,874,881,888,895,902,909,915,922,929,936,943,950,957,964,970,977,984,991,997,1004,1011,1018,1024,1031,1038,1044,1051,1057,1064,1071,1077,1084,1090,1097,1103,1110,1116,1123,1129,1136,1142,1149,1155,1161,1168,
1174,1180,1187,1193,1199,1205,1212,1218,1224,1230,1236,1242,1249,1255,1261,1267,1273,1279,1285,1291,1297,1303,1309,1315,1321,1327,1333,1338,1344,1350,1356,1362,1367,1373,1379,1384,1390,1396,1401,1407,1413,1418,1424,1429,1435,
1440,1446,1451,1457,1462,1468,1473,1478,1484,1489,1494,1499,1505,1510,1515,1520,1525,1531,1536,1541,1546,1551,1556,1561,1566,1571,1576,1581,1586,1591,1595,1600,1605,1610,1615,1619,1624,1629,1633,1638,1643,1647,1652,1656,1661,
1665,1670,1674,1679,1683,1688,1692,1696,1701,1705,1709,1713,1718,1722,1726,1730,1734,1738,1742,1746,1750,1754,1758,1762,1766,1770,1774,1778,1782,1785,1789,1793,1797,1800,1804,1808,1811,1815,1818,1822,1825,1829,1832,1836,1839,
1843,1846,1849,1853,1856,1859,1862,1866,1869,1872,1875,1878,1881,1884,1887,1890,1893,1896,1899,1902,1905,1907,1910,1913,1916,1918,1921,1924,1926,1929,1932,1934,1937,1939,1942,1944,1946,1949,1951,1954,1956,1958,1960,1963,1965,
1967,1969,1971,1973,1975,1977,1979,1981,1983,1985,1987,1989,1991,1992,1994,1996,1998,1999,2001,2003,2004,2006,2007,2009,2010,2012,2013,2015,2016,2017,2019,2020,2021,2022,2023,2025,2026,2027,2028,2029,2030,2031,2032,2033,2034,
2035,2036,2036,2037,2038,2039,2039,2040,2041,2041,2042,2042,2043,2043,2044,2044,2045,2045,2046,2046,2046,2046,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2046,2046,2046,2046,2045,2045,2045,
2044,2044,2043,2043,2042,2041,2041,2040,2040,2039,2038,2037,2037,2036,2035,2034,2033,2032,2031,2030,2029,2028,2027,2026,2025,2024,2023,2021,2020,2019,2018,2016,2015,2014,2012,2011,2009,2008,2006,2005,2003,2001,2000,1998,1996,
1995,1993,1991,1989,1988,1986,1984,1982,1980,1978,1976,1974,1972,1970,1968,1965,1963,1961,1959,1957,1954,1952,1950,1947,1945,1942,1940,1937,1935,1932,1930,1927,1925,1922,1919,1917,1914,1911,1908,1906,1903,1900,1897,1894,1891,
1888,1885,1882,1879,1876,1873,1870,1867,1863,1860,1857,1854,1850,1847,1844,1840,1837,1834,1830,1827,1823,1820,1816,1812,1809,1805,1802,1798,1794,1790,1787,1783,1779,1775,1771,1768,1764,1760,1756,1752,1748,1744,1740,1736,1731,
1727,1723,1719,1715,1711,1706,1702,1698,1693,1689,1685,1680,1676,1671,1667,1662,1658,1653,1649,1644,1640,1635,1630,1626,1621,1616,1611,1607,1602,1597,1592,1587,1582,1578,1573,1568,1563,1558,1553,1548,1543,1537,1532,1527,1522,
1517,1512,1506,1501,1496,1491,1485,1480,1475,1469,1464,1459,1453,1448,1442,1437,1431,1426,1420,1415,1409,1403,1398,1392,1386,1381,1375,1369,1363,1358,1352,1346,1340,1334,1329,1323,1317,1311,1305,1299,1293,1287,1281,1275,1269,
1263,1257,1251,1245,1238,1232,1226,1220,1214,1207,1201,1195,1189,1182,1176,1170,1163,1157,1151,1144,1138,1131,1125,1119,1112,1106,1099,1093,1086,1079,1073,1066,1060,1053,1046,1040,1033,1026,1020,1013,1006,1000,993,986,979,973,
966,959,952,945,938,932,925,918,911,904,897,890,883,876,869,862,855,848,841,834,827,820,813,806,799,792,784,777,770,763,756,749,741,734,727,720,713,705,698,691,684,676,669,662,654,647,640,632,625,618,610,603,596,588,581,573,566,
559,551,544,536,529,521,514,506,499,491,484,476,469,461,454,446,439,431,424,416,409,401,393,386,378,371,363,355,348,340,333,325,317,310,302,294,287,279,271,264,256,249,241,233,226,218,210,202,195,187,179,172,164,156,149,141,133,
126,118,110,102,95,87,79,72,64,56,48,41,33,25,18,10,2,-6,-13,-21,-29,-37,-44,-52,-60,-67,-75,-83,-91,-98,-106,-114,-121,-129,-137,-145,-152,-160,-168,-175,-183,-191,-198,-206,-214,-221,-229,-237,-244,-252,-260,-267,-275,-283,
-290,-298,-306,-313,-321,-329,-336,-344,-351,-359,-367,-374,-382,-389,-397,-404,-412,-420,-427,-435,-442,-450,-457,-465,-472,-480,-487,-495,-502,-510,-517,-525,-532,-540,-547,-555,-562,-569,-577,-584,-592,-599,-606,-614,-621,
-628,-636,-643,-650,-658,-665,-672,-680,-687,-694,-701,-709,-716,-723,-730,-738,-745,-752,-759,-766,-774,-781,-788,-795,-802,-809,-816,-823,-830,-837,-844,-851,-858,-866,-872,-879,-886,-893,-900,-907,-914,-921,-928,-935,-942,
-949,-955,-962,-969,-976,-983,-989,-996,-1003,-1010,-1016,-1023,-1030,-1036,-1043,-1050,-1056,-1063,-1069,-1076,-1083,-1089,-1096,-1102,-1109,-1115,-1122,-1128,-1135,-1141,-1147,-1154,-1160,-1167,-1173,-1179,-1185,-1192,-1198,
-1204,-1211,-1217,-1223,-1229,-1235,-1241,-1248,-1254,-1260,-1266,-1272,-1278,-1284,-1290,-1296,-1302,-1308,-1314,-1320,-1326,-1332,-1337,-1343,-1349,-1355,-1361,-1366,-1372,-1378,-1384,-1389,-1395,-1401,-1406,-1412,-1417,-1423,
-1428,-1434,-1440,-1445,-1450,-1456,-1461,-1467,-1472,-1477,-1483,-1488,-1493,-1499,-1504,-1509,-1514,-1520,-1525,-1530,-1535,-1540,-1545,-1550,-1555,-1560,-1565,-1570,-1575,-1580,-1585,-1590,-1595,-1600,-1604,-1609,-1614,-1619,
-1623,-1628,-1633,-1637,-1642,-1647,-1651,-1656,-1660,-1665,-1669,-1674,-1678,-1683,-1687,-1691,-1696,-1700,-1704,-1709,-1713,-1717,-1721,-1726,-1730,-1734,-1738,-1742,-1746,-1750,-1754,-1758,-1762,-1766,-1770,-1774,-1778,-1781,
-1785,-1789,-1793,-1796,-1800,-1804,-1807,-1811,-1815,-1818,-1822,-1825,-1829,-1832,-1836,-1839,-1842,-1846,-1849,-1852,-1856,-1859,-1862,-1865,-1869,-1872,-1875,-1878,-1881,-1884,-1887,-1890,-1893,-1896,-1899,-1902,-1905,-1908,
-1910,-1913,-1916,-1919,-1921,-1924,-1927,-1929,-1932,-1934,-1937,-1939,-1942,-1944,-1947,-1949,-1951,-1954,-1956,-1958,-1961,-1963,-1965,-1967,-1969,-1971,-1974,-1976,-1978,-1980,-1982,-1984,-1985,-1987,-1989,-1991,-1993,-1995,
-1996,-1998,-2000,-2001,-2003,-2005,-2006,-2008,-2009,-2011,-2012,-2014,-2015,-2016,-2018,-2019,-2020,-2022,-2023,-2024,-2025,-2026,-2028,-2029,-2030,-2031,-2032,-2033,-2034,-2035,-2035,-2036,-2037,-2038,-2039,-2039,-2040,-2041,
-2041,-2042,-2043,-2043,-2044,-2044,-2045,-2045,-2046,-2046,-2046,-2047,-2047,-2047,-2048,-2048,-2048,-2048,-2048,-2048,-2048,-2048,-2048
};

int sin60( int t )
{
    int t4, sel;
    int val;
    t4 = t / 10; // units of 4usec, 
    t4 = t4 % (SINE_LIMIT*4); // 4 repeats of .75 cycles = 3 cycles total
    sel = t4 / SINE_LIMIT;
    t4 = t4 % SINE_LIMIT;
    switch( sel ) {
        case 0: val  = sine_60hz[t4]; break;
        case 1: val  = sine_60hz[SINE_LIMIT-t4]; break;
        case 2: val = -sine_60hz[t4]; break;
        case 3: val = -sine_60hz[SINE_LIMIT-t4]; break;
    }
    return( val );
}

int cos60( int t )
{
    int t4, sel;
    int val;
    t4 = t / 10; // units of 4usec, 
    t4 = t4 % (SINE_LIMIT*4); // 4 repeats of .75 cycles = 3 cycles total
    sel = t4 / SINE_LIMIT;
    t4 = t4 % SINE_LIMIT;
    switch( sel ) {
        case 0: val = -sine_60hz[SINE_LIMIT-t4]; break;
        case 1: val  = sine_60hz[t4]; break;
        case 2: val  = sine_60hz[SINE_LIMIT-t4]; break;
        case 3: val = -sine_60hz[t4]; break;
    }
    return( val );
}

float rms_data( int npoints, int16_t *sample, int16_t *timestamp, int log_flag )
// given npoints, with voltage scale, and array of sample, as well as usec timestamps
// a) fit a sine wave to the curve
// b) calculate RMS
{
    int ii, isin, icos;
    int iMtX0, iMtX1, iMtM00, iMtM01, iMtM10, iMtM11;
    iMtX0 = iMtX1 = 0;
    iMtM00 = iMtM01 = iMtM10 = iMtM11 = 0;
    // build MtM and MtX linear equations
    for( ii = 0; ii < npoints; ii++ ) {
        isin = sin60( timestamp[ii] );
        icos = cos60( timestamp[ii] );
        //printf("sample[%d] = { T=%d, S=%d, isin=%d, icos=%d\n", ii, timestamp[ii], sample[ii], isin, icos );
        iMtM00 += isin*isin;
        iMtM01 += icos*isin;
        iMtM10 += isin*icos;
        iMtM11 += icos*icos;
        iMtX0  += isin*sample[ii];
        iMtX1  += icos*sample[ii];
    }
    //pc.printf("iMtM00 %d iMtM01 %d iMtM10 %d iMtM11 %d iMtX0 %d iMtX1 %d \n", iMtM00, iMtM01, iMtM10, iMtM11, iMtX0, iMtX1 ); 
    // Scale
    float fMtX0, fMtX1, fMtM00, fMtM01, fMtM10, fMtM11;
    fMtX0  = (float)iMtX0*ADC_VOLTAGE_MAG/(float)(SINE_MAG*ADC_SINE_MAG);
    fMtX1  = (float)iMtX1*ADC_VOLTAGE_MAG/(float)(SINE_MAG*ADC_SINE_MAG);
    fMtM00 = (float)iMtM00/(float)(SINE_MAG*SINE_MAG);
    fMtM01 = (float)iMtM01/(float)(SINE_MAG*SINE_MAG);
    fMtM10 = (float)iMtM10/(float)(SINE_MAG*SINE_MAG);
    fMtM11 = (float)iMtM11/(float)(SINE_MAG*SINE_MAG);
 
    // Invert matric    
    float fdet;
    float fMtMi00, fMtMi01, fMtMi10, fMtMi11;

    fdet = fMtM00 * fMtM11 - fMtM01 * fMtM10;
    
    if( fdet <= MIN_DET_VAL )
        return( RMS_ERROR_VAL );
    
    fMtMi00 =  fMtM11 / fdet;
    fMtMi01 = -fMtM10 / fdet;
    fMtMi10 = -fMtM01 / fdet;
    fMtMi11 =  fMtM00 / fdet;
    
    // Multiply MtMi * MtX to get output A,B
    float a, b;
    a = (fMtMi00 * fMtX0 + fMtMi01 * fMtX1);
    b = (fMtMi10 * fMtX0 + fMtMi11 * fMtX1);
 
    // Log the data
    if( log_flag ) {
        printf("A = %f, B = %f\n", a, b );
        for( ii = 0; ii < npoints; ii++ ) {
            printf("sample[%d] = { T=%d, S=%d, a*isin=%f, b*icos=%f\n", ii, timestamp[ii], sample[ii], a*sin60( timestamp[ii] ), b*cos60( timestamp[ii] ) );  
        }  
    }
    // Calc RMS
    float rms;
    rms = sqrt( a*a/2 + b*b/2 );
    
    return( rms );
}

#define MAX_60HZ_SAMPLES        250
#define MAX_60HZ_USEC           16666

int16_t sample_value[MAX_60HZ_SAMPLES];
int16_t sample_times[MAX_60HZ_SAMPLES];


int sample_60hz( int16_t *time, int16_t *sample )
{

    Timer timer;
    int count, sample_time;
    timer.start();
  
    int ref;
    int meas;
    ref = adc_ref.read_u16(); // range 0 - 65535
    count = 0;
    sample_time = 0;
    timer.reset();
    while( count < MAX_60HZ_SAMPLES && sample_time < 4*MAX_60HZ_USEC ) {
        meas = adc_in.read_u16();
        sample_value[count] = meas - ref;
        sample_times[count] = sample_time = timer.read_us();
        count++;
    }
    return( count );
} 

const char display_char[] = "0123456789aAbBcCdDeEfFhHiIjJlLnNoOpPrRuUyY_=- .\"'`[]";
const int  segment_char[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x77, 0x7C, 0x7C, 0x58, 0x39, 0x5E, 0x5E, 
                                0x79, 0x79, 0x71, 0x71, 0x74, 0x76, 0x10, 0x30, 0x1E, 0x1E, 0x30, 0x38, 0x54, 0x54, 0x5C, 0x3F, 0x73, 0x73, 
                                0x50, 0x50, 0x1C, 0x3E, 0x6E, 0x6E, 0x08, 0x48, 0x40, 0x00, 0x80, 0x22, 0x02, 0x20, 0x39, 0x0F };

void segment_display( char *text )
{
    int ptr, idx;
    int segment_word;
    int segment_shift;
    
    //printf("LED Text:[%s]\n", text );
    segment_word = 0;
    segment_shift = 0;
    for( ptr = 0; text[ptr] != '\0'; ptr++ ) {
        for( idx = 0; display_char[idx] != '\0'; idx++ ) {
            if( display_char[idx] == text[ptr] ) {
                //printf("Found  '%c'\n", display_char[idx] );
                if( display_char[idx] != '.' && ptr != 0 )
                    segment_shift += 8;
                segment_word = segment_word | ( segment_char[idx]<<segment_shift );
            }
        }
    }
    //printf("Seg = %08x\n", segment_word );
    // Enable Display
    LTM_8522HR_DATAENABLE = 0;
    LTM_8522HR_DATA = 0;
    LTM_8522HR_CLOCK = 0;
    // Start bit
    LTM_8522HR_DATA  = 1; wait_us( 1 );
    LTM_8522HR_CLOCK = 1; wait_us( 1 );
    LTM_8522HR_CLOCK = 0; wait_us( 1 );
    // Shift out 36 bits
    for( int ii = 0; ii < 36; ii++ ) {
        LTM_8522HR_DATA  = ( segment_word >> ii ) & 1; wait_us( 1 );
        LTM_8522HR_CLOCK = 1; wait_us( 1 );
        LTM_8522HR_CLOCK = 0; wait_us( 1 );
    }
}

void display_current( float val )
{
    char str[10];
    sprintf( str, "%4.1f", val );
    segment_display( str );
}
   
int main()
{
    float val; // current reading, in Amps
    float ref; // Minimum pump current threshold, in Amps
    int count;
    int state, prev_state;
    int display_idx;
    int button_hold;
    Timer timeout;
    timeout.start();
    printf("\nSTM32 ADC external channels reading example\n");
    segment_display( "Pon" );
    {
        wait( 0.5 );
        segment_display( "8.88" );
        button_led = 1;
        wait( 0.5 );
        segment_display( "88.8" );
        button_led = 0;
        wait( 0.5 );
        segment_display( "888." );
        button_led = 1;
        wait( 0.5 );
        segment_display( "8.8.8." );
        button_led = 0;
        wait( 0.5 );
    }
    {
        int read;
        read = thresh.read_u16();
        val = read * 20.0/65536;
        printf("Ref Val [%d] V = %f4.1\n", read, val );
        segment_display("Ref");
        wait( 0.5 );   
        timeout.reset();
        while( timeout.read() < 5.0 || button ) {     // 5 sec to adjust ref, or hold button to extend
            button_led = 1;
            display_current( thresh.read_u16() * 20.0/65536 );
        }
        button_led = 0;
    }
        
    printf("Sample Start\n");
    count = sample_60hz( sample_times, sample_value );
    printf("Sample Stop, %d samples\n", count);
    //for( int ii = 0; ii < count; ii++ ) {
    //    printf("[%d] t=%d s=%d\n", ii, sample_times[ii], sample_value[ii] ); 
    //}
    printf("\n");
    val = rms_data( count, sample_value, sample_times, 0 );
    printf("RMS voltage = %f \n", val );
    wait(0.1);    
    state = 0;
    //relay = 1;
    timeout.reset();

    // Init Statistic variables
    float pump_duration = 0.0;
    float peak_current = 0.0;
    int count_current = 0;
    float avg_current = 0.0;

    while(1) {
        val = 0.0;
        ref = 0.0;
        for( int ii = 0; ii < 10; ii++ ) {
            count = sample_60hz( sample_times, sample_value );
            val += rms_data( count, sample_value, sample_times, 0 );
            ref = ref + (thresh.read_u16() * 20.0/65536);
        }
        button_hold = button;
        // scale to amps
        val = val * 10;
        ref = ref / 10;
        printf("RMS voltage = %5.2f \n", val );

        //////////////////////////        
        // State Transitions
        //////////////////////////        

        prev_state = state;
        if( timeout.read() > 7.0 && state == 0 && button_hold == 0) { // error timeout pump did not hit current
            state = 3;
            printf("Pump Error = %5.2f\n", val);
        }
        else if( val > ref && state == 0 ) { // Sucessful Pump Startup
            state = 1;
            printf("Pumping Detect = %5.2f\n", val);
        }
        else if( val < ref && state == 1 && button_hold == 0 ) { // Sucessful Pump to empty and Stop
            state = 2;
            printf("Pump Empty = %5.2f\n", val);
        }    
        else if( timeout.read() > 180.0 && state == 1 ) { // 3 mins is long enough but not a real error
            state = 2;
            printf("Pump Timeout = %5.2f\n", val);
        }    
        else if( state == 2 && timeout.read() > (24.0 * 60.0 * 60.0) ) { // Pump again in 24 hours
            state = 0;
            printf("Pump Time = %5.2f\n", val);           
        }
        else if( state == 2 && button_hold == 1 ) { // Pump again now
            state = 0;
            printf("Pump Manual = %5.2f\n", val);           
        }
        else if( state == 3 && button_hold == 1 ) { // Force Pump Over-ride
            state = 4;
            printf("Pumping Override = %5.2f\n", val);
        }
        else if( state == 4 && val > ref ) { // Sucessful Pump Recovery from Error
            state = 1;
            printf("Pump Recover = %5.2f\n", val);
        }
        else if( state == 4 && button_hold == 0 ) { // Failed Recovery from Error
            state = 3;
            printf("Pump Error = %5.2f\n", val);
        }

        //////////////////////////        
        // State Transition Actions
        //////////////////////////        

        if( prev_state != state ) {
            switch( state ) {
                // Init Statis
                case 0: // pump start
                case 1: // pumping detected
                case 4: // err over-ride
                    peak_current = 0.0;
                    count_current = 0;
                    avg_current = 0.0;
                    timeout.reset();
                    break;
                // Calc Display status
                case 2: // Fine case
                case 3: // Err case  
                    display_idx = 0;
                    avg_current = avg_current / count_current;
                    pump_duration = timeout.read();
                    timeout.reset();
                    break;
            }   
        }                    


        //////////////////////////        
        // State Stat Accumulation
        //////////////////////////

        switch( state ) {
            case 0: // pump start
            case 1: // pumping detected
            case 4: // err over-ride
                avg_current += val;
                count_current += 1;
                peak_current = ( val > peak_current ) ? val : peak_current;
                break;
        }   
 
        //////////////////////////        
        // State Display
        //////////////////////////
           
        switch( state ) {
            case 0: // pump start
            case 1: // pumping
            case 4: // err over-ride
                display_current( val );
                break;
            case 2: // Finished pumping, wait 24 hours to restart
                switch( display_idx ) {
                    case 0:
                    case 1:
                    case 2:
                    case 3: segment_display( "Fin" );                   break;
                    case 4: segment_display( "ELA" );                   break;
                    case 5: display_current( timeout.read() / 3600.0 ); break;
                    case 6: segment_display( "dur" );                   break;
                    case 7: display_current( pump_duration );           break;
                    case 8: segment_display( "rEF" );                   break;
                    case 9: display_current( ref );                     break;
                    case 10: segment_display( "PPC" );                  break;
                    case 11: display_current( peak_current );           break;
                    case 12: segment_display( "APC" );                  break;
                    case 13: display_current( avg_current );            break;
                    case 14: segment_display( "Cur" );                  break;
                    case 15: display_current( val );                    break;
                }
                display_idx = ( display_idx + 1 ) % 16;
                break;
            case 3: // Error, low pump current.
                switch( display_idx ) {
                    case 0:
                    case 1:
                    case 2:
                    case 3: segment_display( "Err" );                   break;
                    case 4: segment_display( "ELA" );                   break;
                    case 5: display_current( timeout.read() / 3600.0 ); break;
                    case 6: segment_display( "dur" );                   break;
                    case 7: display_current( pump_duration );           break;
                    case 8: segment_display( "rEF" );                   break;
                    case 9: display_current( ref );                     break;
                    case 10: segment_display( "PPC" );                  break;
                    case 11: display_current( peak_current );           break;
                    case 12: segment_display( "APC" );                  break;
                    case 13: display_current( avg_current );            break;
                    case 14: segment_display( "Cur" );                  break;
                    case 15: display_current( val );                    break;
                }
                display_idx = ( display_idx + 1 ) % 16;
                break;
        } // case state
        
        //////////////////////////        
        // State Outputs / Pump
        //////////////////////////
         
        // Button LED 
        // ON = ok pumping, BLINK = low pump current
        button_led = ( state == 1 ) ? 1 : ( state == 0 || state == 4 ) ? !button_led : 0; 

        // Pump Relay
        // ON in state == 0 or 1, and 3 if button
        relay = ( state == 1 || state == 0 || state == 4 ) ? 1 : 0; // pump on in states 0, 1
        
        // Led tell-tale
        led = !led;
    }
}
