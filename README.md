# Effluent-pump
Never go down there again! Effluent pump float switch replacement based on current measurement. 

### Float switch.
These things seem to fail, and replacement invoves going down there (nope). Mine failed, and I used a 
bypsss switch to pump it out daily while waiting for a warantee replacement. I added a simple current 
transformer and meter and watching it during the daily pumpout and decided to automate it, rather than
replace the switch.

### Automation Sequence
Control the pump with a relay and digitize the current measurements. 
The RMS current drops when the pump has emptied the tank.
Every 24 hours, the controller will pump for up to 180 seconds or until the current drops.
When the pump current drops below a reference threshold the pump is turned off.
At any time pressing the button starts an immediate pump operation.

It is in daily use.

![Effluent pump controller](image.png "Pump Controller")

### Pleasantly surprising features:
Things which worked out in a satisfying maner, and I've employed them on other projects.

#### RMS Current Measurement
RMS current calculation is based on an 12 bit sin table of a 60 Hz wave sampled at an 1usec.
Folds the table for small microcontroller use with integer accumulation for RMS calculation.
Table folding is based on .75 of a 60 Hz cycle is exactly 0.0125 seconds, so a table of 1250 entries is exact. This makes it easy to 
generate sin values based on sw sample time. The allows a least squared curve fitting of a 60 Hz sin wave to timestamped 
samples and ultimately extracting a stable RMS current value. The rms_data() function performs this.

```
float rms_data( int npoints, int16_t *sample, int16_t *timestamp, int log_flag )
// given npoints, with voltage scale, and array of sample, as well as usec timestamps
// a) fit a sine wave to the curve
// b) calculate RMS
```

#### Everything at a glance display
While waiting for 24 hours the limited display, 3 digits of 7 segments, cycled through a set
of mnemonics and values which displayed how long ago it pumped, and for how long, as well as a few other values.
This assured me that all is working at a glance, even from across the room.
One of the values is the reference value, allowing precise setting of the potentiometer.

The mnemonics are chosen to be displayable on the 7 seg display with display_idx incrementing every 1/2 sec.
```
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
```

### Implementation
This was implement using the (now defunct) online MBED devlopement environment. However I would use this c code as a starting point
if I need to re-implent it.
