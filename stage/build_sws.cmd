@echo off
del pbjbase.sw
del pbjconfig.sw

sw __pbjbase__ create   pbjbase.sw
sw __pbjbase__ prop     description      "PBJsimple Base Assets"
sw __pbjbase__ prop     copyright        "(c) 2013 PBJ^2 Productions"
sw __pbjbase__ prop     version          0.2
sw __pbjbase__ font     std_font                ../assets/std.xml
sw __pbjbase__ texture  std_font.texture       ../assets/std_0.png
sw __pbjbase__ texture  player_outline.texture ../assets/player_outline.png
btngen std_btn       0.5  0.6  0.65 | sqlite3 pbjbase.sw
btngen highlight_btn 0.6  0.5  0.45 | sqlite3 pbjbase.sw

sw __pbjbase__ material black         0    0    0
sw __pbjbase__ material black1x1      0    0    0
sw __pbjbase__ material black2x1      0    0    0
sw __pbjbase__ material black4x1      0    0    0
sw __pbjbase__ material black8x1      0    0    0
sw __pbjbase__ material black2x2      0    0    0
sw __pbjbase__ material black4x2      0    0    0
sw __pbjbase__ material black8x2      0    0    0
sw __pbjbase__ material black16x2     0    0    0
sw __pbjbase__ material black4x4      0    0    0
sw __pbjbase__ material black8x4      0    0    0
sw __pbjbase__ material black16x4     0    0    0
sw __pbjbase__ material black8x8      0    0    0
sw __pbjbase__ material black16x8     0    0    0
sw __pbjbase__ material black16x16    0    0    0
                                      
sw __pbjbase__ material white         1    1    1
sw __pbjbase__ material white1x1      1    1    1
sw __pbjbase__ material white2x1      1    1    1
sw __pbjbase__ material white4x1      1    1    1
sw __pbjbase__ material white8x1      1    1    1
sw __pbjbase__ material white2x2      1    1    1
sw __pbjbase__ material white4x2      1    1    1
sw __pbjbase__ material white8x2      1    1    1
sw __pbjbase__ material white16x2     1    1    1
sw __pbjbase__ material white4x4      1    1    1
sw __pbjbase__ material white8x4      1    1    1
sw __pbjbase__ material white16x4     1    1    1
sw __pbjbase__ material white8x8      1    1    1
sw __pbjbase__ material white16x8     1    1    1
sw __pbjbase__ material white16x16    1    1    1

sw __pbjbase__ material red           0.9  0.1  0
sw __pbjbase__ material red1x1        0.9  0.1  0
sw __pbjbase__ material red2x1        0.9  0.1  0
sw __pbjbase__ material red4x1        0.9  0.1  0
sw __pbjbase__ material red8x1        0.9  0.1  0
sw __pbjbase__ material red2x2        0.9  0.1  0
sw __pbjbase__ material red4x2        0.9  0.1  0
sw __pbjbase__ material red8x2        0.9  0.1  0
sw __pbjbase__ material red16x2       0.9  0.1  0
sw __pbjbase__ material red4x4        0.9  0.1  0
sw __pbjbase__ material red8x4        0.9  0.1  0
sw __pbjbase__ material red16x4       0.9  0.1  0
sw __pbjbase__ material red8x8        0.9  0.1  0
sw __pbjbase__ material red16x8       0.9  0.1  0
sw __pbjbase__ material red16x16      0.9  0.1  0

sw __pbjbase__ material orange        0.8  0.4  0
sw __pbjbase__ material orange1x1     0.8  0.4  0
sw __pbjbase__ material orange2x1     0.8  0.4  0
sw __pbjbase__ material orange4x1     0.8  0.4  0
sw __pbjbase__ material orange8x1     0.8  0.4  0
sw __pbjbase__ material orange2x2     0.8  0.4  0
sw __pbjbase__ material orange4x2     0.8  0.4  0
sw __pbjbase__ material orange8x2     0.8  0.4  0
sw __pbjbase__ material orange16x2    0.8  0.4  0
sw __pbjbase__ material orange4x4     0.8  0.4  0
sw __pbjbase__ material orange8x4     0.8  0.4  0
sw __pbjbase__ material orange16x4    0.8  0.4  0
sw __pbjbase__ material orange8x8     0.8  0.4  0
sw __pbjbase__ material orange16x8    0.8  0.4  0
sw __pbjbase__ material orange16x16   0.8  0.4  0

sw __pbjbase__ material yellow        1    1    0
sw __pbjbase__ material yellow1x1     1    1    0
sw __pbjbase__ material yellow2x1     1    1    0
sw __pbjbase__ material yellow4x1     1    1    0
sw __pbjbase__ material yellow8x1     1    1    0
sw __pbjbase__ material yellow2x2     1    1    0
sw __pbjbase__ material yellow4x2     1    1    0
sw __pbjbase__ material yellow8x2     1    1    0
sw __pbjbase__ material yellow16x2    1    1    0
sw __pbjbase__ material yellow4x4     1    1    0
sw __pbjbase__ material yellow8x4     1    1    0
sw __pbjbase__ material yellow16x4    1    1    0
sw __pbjbase__ material yellow8x8     1    1    0
sw __pbjbase__ material yellow16x8    1    1    0
sw __pbjbase__ material yellow16x16   1    1    0

sw __pbjbase__ material lime          0.3  0.9  0
sw __pbjbase__ material lime1x1       0.3  0.9  0
sw __pbjbase__ material lime2x1       0.3  0.9  0
sw __pbjbase__ material lime4x1       0.3  0.9  0
sw __pbjbase__ material lime8x1       0.3  0.9  0
sw __pbjbase__ material lime2x2       0.3  0.9  0
sw __pbjbase__ material lime4x2       0.3  0.9  0
sw __pbjbase__ material lime8x2       0.3  0.9  0
sw __pbjbase__ material lime16x2      0.3  0.9  0
sw __pbjbase__ material lime4x4       0.3  0.9  0
sw __pbjbase__ material lime8x4       0.3  0.9  0
sw __pbjbase__ material lime16x4      0.3  0.9  0
sw __pbjbase__ material lime8x8       0.3  0.9  0
sw __pbjbase__ material lime16x8      0.3  0.9  0
sw __pbjbase__ material lime16x16     0.3  0.9  0

sw __pbjbase__ material green         0    0.5  0
sw __pbjbase__ material green1x1      0    0.5  0
sw __pbjbase__ material green2x1      0    0.5  0
sw __pbjbase__ material green4x1      0    0.5  0
sw __pbjbase__ material green8x1      0    0.5  0
sw __pbjbase__ material green2x2      0    0.5  0
sw __pbjbase__ material green4x2      0    0.5  0
sw __pbjbase__ material green8x2      0    0.5  0
sw __pbjbase__ material green16x2     0    0.5  0
sw __pbjbase__ material green4x4      0    0.5  0
sw __pbjbase__ material green8x4      0    0.5  0
sw __pbjbase__ material green16x4     0    0.5  0
sw __pbjbase__ material green8x8      0    0.5  0
sw __pbjbase__ material green16x8     0    0.5  0
sw __pbjbase__ material green16x16    0    0.5  0

sw __pbjbase__ material cyan          0    1    0.8
sw __pbjbase__ material cyan1x1       0    1    0.8
sw __pbjbase__ material cyan2x1       0    1    0.8
sw __pbjbase__ material cyan4x1       0    1    0.8
sw __pbjbase__ material cyan8x1       0    1    0.8
sw __pbjbase__ material cyan2x2       0    1    0.8
sw __pbjbase__ material cyan4x2       0    1    0.8
sw __pbjbase__ material cyan8x2       0    1    0.8
sw __pbjbase__ material cyan16x2      0    1    0.8
sw __pbjbase__ material cyan4x4       0    1    0.8
sw __pbjbase__ material cyan8x4       0    1    0.8
sw __pbjbase__ material cyan16x4      0    1    0.8
sw __pbjbase__ material cyan8x8       0    1    0.8
sw __pbjbase__ material cyan16x8      0    1    0.8
sw __pbjbase__ material cyan16x16     0    1    0.8
                                      
sw __pbjbase__ material blue          0    0.3  1  
sw __pbjbase__ material blue1x1       0    0.3  1
sw __pbjbase__ material blue2x1       0    0.3  1
sw __pbjbase__ material blue4x1       0    0.3  1
sw __pbjbase__ material blue8x1       0    0.3  1
sw __pbjbase__ material blue2x2       0    0.3  1
sw __pbjbase__ material blue4x2       0    0.3  1
sw __pbjbase__ material blue8x2       0    0.3  1
sw __pbjbase__ material blue16x2      0    0.3  1
sw __pbjbase__ material blue4x4       0    0.3  1
sw __pbjbase__ material blue8x4       0    0.3  1
sw __pbjbase__ material blue16x4      0    0.3  1
sw __pbjbase__ material blue8x8       0    0.3  1
sw __pbjbase__ material blue16x8      0    0.3  1
sw __pbjbase__ material blue16x16     0    0.3  1

sw __pbjbase__ material indigo        0.4  0    0.8    
sw __pbjbase__ material indigo1x1     0.4  0    0.8
sw __pbjbase__ material indigo2x1     0.4  0    0.8
sw __pbjbase__ material indigo4x1     0.4  0    0.8
sw __pbjbase__ material indigo8x1     0.4  0    0.8
sw __pbjbase__ material indigo2x2     0.4  0    0.8
sw __pbjbase__ material indigo4x2     0.4  0    0.8
sw __pbjbase__ material indigo8x2     0.4  0    0.8
sw __pbjbase__ material indigo16x2    0.4  0    0.8
sw __pbjbase__ material indigo4x4     0.4  0    0.8
sw __pbjbase__ material indigo8x4     0.4  0    0.8
sw __pbjbase__ material indigo16x4    0.4  0    0.8
sw __pbjbase__ material indigo8x8     0.4  0    0.8
sw __pbjbase__ material indigo16x8    0.4  0    0.8
sw __pbjbase__ material indigo16x16   0.4  0    0.8

sw __pbjbase__ material magenta       1    0.2  0.6
sw __pbjbase__ material magenta1x1    1    0.2  0.6
sw __pbjbase__ material magenta2x1    1    0.2  0.6
sw __pbjbase__ material magenta4x1    1    0.2  0.6
sw __pbjbase__ material magenta8x1    1    0.2  0.6
sw __pbjbase__ material magenta2x2    1    0.2  0.6
sw __pbjbase__ material magenta4x2    1    0.2  0.6
sw __pbjbase__ material magenta8x2    1    0.2  0.6
sw __pbjbase__ material magenta16x2   1    0.2  0.6
sw __pbjbase__ material magenta4x4    1    0.2  0.6
sw __pbjbase__ material magenta8x4    1    0.2  0.6
sw __pbjbase__ material magenta16x4   1    0.2  0.6
sw __pbjbase__ material magenta8x8    1    0.2  0.6
sw __pbjbase__ material magenta16x8   1    0.2  0.6
sw __pbjbase__ material magenta16x16  1    0.2  0.6

sw __pbjbase__ material spawnpoint 1    0.25 0    0.66
sw __pbjbase__ material player     0.8  0.2  0
sw __pbjbase__ material bots       0.5  0.9  0
sw __pbjbase__ material bullet     0    0.8  1
sw __pbjbase__ material terrain    1    1    0.2

sw __pbjbase__ material terrain_big 1 1 1

sw __pbjbase__ material player1_outline 1.0000 0.0700 0.3800 1.0000 player_outline.texture modulate
sw __pbjbase__ material player2_outline 0.5300 1.0000 0.0700 1.0000 player_outline.texture modulate
sw __pbjbase__ material player3_outline 0.8600 1.0000 0.0700 1.0000 player_outline.texture modulate
sw __pbjbase__ material player4_outline 1.0000 0.3100 0.0700 1.0000 player_outline.texture modulate
sw __pbjbase__ material player5_outline 0.0700 1.0000 0.5500 1.0000 player_outline.texture modulate

sw __pbjbase__ sound	bgmusic    ../assets/sounds/bgmusic.wav
sw __pbjbase__ sound	wpnfire    ../assets/sounds/weapon_fire.wav
sw __pbjbase__ sound	dmg        ../assets/sounds/snd_dmg.wav
sw __pbjbase__ sound	death      ../assets/sounds/snd_death.wav
sw __pbjbase__ sound	jump       ../assets/sounds/jump.wav
sw __pbjbase__ sound	menumusic  ../assets/sounds/menumusic.wav

sw __pbjconfig__ create pbjconfig.sw
sw __pbjconfig__ prop   description  "PBJsimple Configuration File"
sw __pbjconfig__ prop   copyright    "(c) 2013 PBJ^2 Productions"
sw __pbjconfig__ prop   version      0.1

sqlite3 pbjbase.sw < pbjbase.sql
sqlite3 pbjconfig.sw < pbjconfig.sql
sw __pbjbase__ vacuum
sw __pbjconfig__ vacuum
