@echo off

del pbjbase.sw
sw __pbjbase__ create pbjbase.sw

sw __pbjbase__ prop description "PBJsimple Base Assets"
sw __pbjbase__ prop copyright "(c) 2013 PBJ^2 Productions"

sw __pbjbase__ font std_font ../assets/std.xml
sw __pbjbase__ texture std_font.texture ../assets/std_0.png

sw __pbjbase__ vacuum


rem del pbjconfig.sw
rem sw __pbjconfig__ create pbjconfig.sw