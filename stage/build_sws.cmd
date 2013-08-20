rem @echo off
del pbjbase.sw
del pbjconfig.sw

sw __pbjbase__ create   pbjbase.sw
sw __pbjbase__ prop     description      "PBJsimple Base Assets"
sw __pbjbase__ prop     copyright        "(c) 2013 PBJ^2 Productions"
sw __pbjbase__ prop     version          0.2
sw __pbjbase__ font     std_font         ../assets/std.xml
sw __pbjbase__ texture  std_font.texture ../assets/std_0.png
btngen std_btn       0.5  0.6  0.65 | sqlite3 pbjbase.sw
btngen highlight_btn 0.6  0.5  0.45 | sqlite3 pbjbase.sw
sw __pbjbase__ material red     0.7961 0.2    0
sw __pbjbase__ material green   0.6    0.9961 0
sw __pbjbase__ material blue    0      0      1
sw __pbjbase__ material cyan    0      0.8    1
sw __pbjbase__ material magenta 0.9922 0.2039 0.6039
sw __pbjbase__ material yellow  1      1      0
sw __pbjbase__ material black   0      0      0
sw __pbjbase__ material white   1      1      1

sw __pbjbase__ material player  0.8    0.2    0
sw __pbjbase__ material bots    0.5    0.9    0
sw __pbjbase__ material bullets 0      0.8    1
sw __pbjbase__ material terrain 1      1      0.2
sw __pbjbase__ sound	bgmusic ../assets/sounds/bgmusic.wav
sw __pbjbase__ sound	wpnfire ../assets/sounds/weapon_fire.wav
sw __pbjbase__ sound	dmg ../assets/sounds/snd_dmg.wav
sw __pbjbase__ sound	death ../assets/sounds/snd_death.wav
sw __pbjbase__ sound	jump ../assets/sounds/jump.wav
sw __pbjbase__ sound	menumusic ../assets/sounds/menumusic.wav

sw __pbjconfig__ create pbjconfig.sw
sw __pbjconfig__ prop   description  "PBJsimple Configuration File"
sw __pbjconfig__ prop   copyright    "(c) 2013 PBJ^2 Productions"
sw __pbjconfig__ prop   version      0.1

sqlite3 pbjbase.sw < pbjbase.sql
sqlite3 pbjconfig.sw < pbjconfig.sql
sw __pbjbase__ vacuum
sw __pbjconfig__ vacuum
