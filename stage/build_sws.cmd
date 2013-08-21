rem @echo off
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
sw __pbjbase__ material red             0.7961 0.2000 0.0000
sw __pbjbase__ material green           0.6000 0.9961 0.0000
sw __pbjbase__ material blue            0.0000 0.0000 1.0000
sw __pbjbase__ material cyan            0.0000 0.8000 1.0000
sw __pbjbase__ material magenta         0.9922 0.2039 0.6039
sw __pbjbase__ material yellow          1.0000 1.0000 0.0000
sw __pbjbase__ material black           0.0000 0.0000 0.0000
sw __pbjbase__ material white           1.0000 1.0000 1.0000

sw __pbjbase__ material player          0.8000 0.2000 0.0000
sw __pbjbase__ material bots            0.5000 0.9000 0.0000
sw __pbjbase__ material bullets         0.0000 0.8000 1.0000
sw __pbjbase__ material terrain         0.6000 0.9961 0.0000

sw __pbjbase__ material player1_outline 1.0000 0.0700 0.3800 1.0000 player_outline.texture modulate
sw __pbjbase__ material player2_outline 0.5300 1.0000 0.0700 1.0000 player_outline.texture modulate
sw __pbjbase__ material player3_outline 0.8600 1.0000 0.0700 1.0000 player_outline.texture modulate
sw __pbjbase__ material player4_outline 1.0000 0.3100 0.0700 1.0000 player_outline.texture modulate
sw __pbjbase__ material player5_outline 0.0700 1.0000 0.5500 1.0000 player_outline.texture modulate

sw __pbjbase__ sound bgmusic ../assets/sounds/bgmusic.wav
sw __pbjbase__ sound wpnfire ../assets/sounds/weapon_fire.wav
sw __pbjbase__ sound dmg ../assets/sounds/snd_dmg.wav
sw __pbjbase__ sound death ../assets/sounds/snd_death.wav
sw __pbjbase__ sound jump ../assets/sounds/jump.wav
sw __pbjbase__ sound menumusic ../assets/sounds/menumusic.wav

sw __pbjconfig__ create pbjconfig.sw
sw __pbjconfig__ prop   description  "PBJsimple Configuration File"
sw __pbjconfig__ prop   copyright    "(c) 2013 PBJ^2 Productions"
sw __pbjconfig__ prop   version      0.1

sqlite3 pbjbase.sw < pbjbase.sql
sqlite3 pbjconfig.sw < pbjconfig.sql
sw __pbjbase__ vacuum
sw __pbjconfig__ vacuum
