@echo off

del pbjbase.sw
sw __pbjbase__ create  pbjbase.sw
sw __pbjbase__ prop    description      "PBJsimple Base Assets"
sw __pbjbase__ prop    copyright        "(c) 2013 PBJ^2 Productions"
sw __pbjbase__ prop    version          0.1
sw __pbjbase__ font    std_font         ../assets/std.xml
sw __pbjbase__ texture std_font.texture ../assets/std_0.png

rem Run Misc SQL queries and minify sandwich
btngen std_btn       0.5  0.6  0.65 | sqlite3 pbjbase.sw
btngen highlight_btn 0.6  0.5  0.45 | sqlite3 pbjbase.sw
sqlite3 pbjbase.sw < pbjbase.sql
sw __pbjbase__ vacuum

del pbjconfig.sw
sw __pbjconfig__ create pbjconfig.sw
sw __pbjconfig__ prop   description  "PBJsimple Configuration File"
sw __pbjconfig__ prop   copyright    "(c) 2013 PBJ^2 Productions"
sw __pbjconfig__ prop   version      0.1

rem Run Misc SQL queries and minify sandwich
sqlite3 pbjconfig.sw < pbjconfig.sql
sw __pbjconfig__ vacuum
