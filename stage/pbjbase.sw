SQLite format 3   @        !           
                                                  -�)   �    ���                                                                                                                                                                                                                                                  �F++�Ctablesw_map_entitiessw_map_entitiesCREATE TABLE sw_map_entities
(
   map_id      INTEGER NOT NULL,
   id          INTEGER PRIMARY KEY,
   entity_type INTEGER NOT NULL,
   rotation    REAL NOT NULL,
   pos_x       REAL NOT NULL,
   pos_y       REAL NOT NULL,
   scale_x     REAL NOT NULL,
   scale_y     REAL NOT NULL
)f�#tablesw_mapssw_mapsCREATE TABLE sw_maps
(
   id   INTEGER PRIMARY KEY,
   name TEXT NOT NULL
)�99�Atablesw_sandwich_propertiessw_sandwich_propertiesCREATE TABLE sw_sandwich_properties
(
   property TEXT PRIMARY KEY, 
   value    NUMERIC
)K_9 indexsqlite_autoindex_sw_sandwich_properties_1sw_sandwich_   	   	      � ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      &Acopyright(c) 2013 PBJ^2 Productions##7descriptionPBJsimple Base AssetsidIg���)k�
   � ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              	id#descriptioncopyright                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               g g                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             �ꖐ��� � 			�PNG

   IHDR   �   �   ����   tEXtSoftware Adobe ImageReadyq�e<  fiTXtXML:com.adobe.xmp     <?xpacket begin="﻿" id="W5M0MpCehiHzreSzNTczkc9d"?> <x:xmpmeta xmlns:x="adobe:ns:meta/" x:xmptk="Adobe XMP Core 5.0-c061 64.140949, 2010/12/07-10:57:01        "> <rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"> <rdf:Description rdf:about="" xmlns:xmpMM="http://ns.ado       R  R �5�� N                                                             �99�Atablesw_sandwich_propertiessw_sandwich_propertiesCREATE TABLE sw_sandwich_properties
(
   property TEXT PRIMARY KEY, 
   value    NUMERIC
)K_9 indexsqlite_autoindex_sw_sandwich_properties_1sw_sandwich_propertiesf�#tablesw_mapssw_mapsCREATE TABLE sw_maps
(
   id   INTEGER PRIMARY KEY,
   name TEXT NOT NULL
)�F++�Ctablesw_map_entitiessw_map_entitiesCREATE TABLE sw_map_entities
(
   map_id      INTEGER NOT NULL,
   id          INTEGER PRIMARY KEY,
   entity_type INTEGER NOT NULL,
   rotation    REAL NOT NULL,
   pos_x       REAL NOT NULL,
   pos_y       REAL NOT NULL,
   scale_x     REAL NOT NULL,
   scale_y     REAL NOT NULL
)�##�utablesw_texturessw_texturesCREATE TABLE sw_textures
(
   id              INTEGER PRIMARY KEY,
   data            NOT NULL,
   internal_format INTEGER NOT NULL,
   srgb            INTEGER NOT NULL,
   mag_filter      INTEGER NOT NULL,
   min_filter      INTEGER NOT NULL
)   � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               ʸ�ּ���~ �X��0��   o o��7                                                               �77�Otablesw_texture_font_charssw_texture_font_chars
CREATE TABLE sw_texture_font_chars
(
   font_id   INTEGER NOT NULL,
   codepoint INTEGER NOT NULL,
   tc_x      INTEGER NOT NULL,
   tc_y      INTEGER NOT NULL,
   tc_width  INTEGER NOT NULL,
   tc_height INTEGER NOT NULL,
   of�(--�tablesw_texture_fontssw_texture_fontsCREATE TABLE sw_texture_fonts
(
   id         INTEGER PRIMARY KEY,
   texture_id NOT NULL,
   cap_height INTEGER NOT NULL
)�77�Otablesw_texture_font_charssw_texture_font_chars
CREATE TABLE sw_texture_font_chars
(
   font_id   INTEGER NOT NULL,
   codepoint INTEGER NOT NULL,
   tc_x      INTEGER NOT NULL,
   tc_y      INTEGER NOT NULL,
   tc_width  INTEGER NOT NULL,
   tc_height INTEGER NOT NULL,
   offset_x  INTEGER NOT NULL,
   offset_y  INTEGER NOT NULL,
   advance   INTEGER NOT NULL,
   PRIMARY KEY (font_id, codepoint)
)I]7 indexsqlite_autoindex_sw_texture_font_chars_1sw_texture_font_chars   �    ������������oU="�����eJ0 � � � � � t Z            #
��]g�A"~@)	"
��]g�A"~??7!
��]g�A"~>I 
��]g�A"~=*V
��]g�A"~<I
��]g�A"~;1G
		��]g�A"~:W
��]g�A"~9j&
��]g�A"~81
��]g�A"~7b&
��]g�A"~6Z&
��]g�A"~5R&
��]g�A"~4J'
��]g�A"~3B'
��]g�A"~2:'
��]g�A"~14G
��]g�A"~02'
��]g�A"~/5
			��]g�A"~.YX
	��]g�A"~-2Z
	��]g�A"~,nR
��]g�A"~+$P
��]g�A"~**P
��]g�A"~)v	
��]g�A"~(f	

	��]g�A"~'[	
��]g�A"~&"(
��]g�A"~%(	
��]g�A"~$$	
��]g�A"~#a6
��]g�A"~"[
		��]g�A"~!~>
		��]g�A"~ WX

		��]g   �l   �K   �+   �
   i   F   #   �    !������zk\M>/ ��������{l]N?0!��������|m^O@1" � � � � � � � �              ��]g�A"~Y<��]g�A"~X;��]g�A"~W:��]g�A"~V9��]g�A"~U8��]g�A"~T7��]g�A"~S6��]g�A"~R5��]g�A"~Q4��]g�A"~P3��]g�A"~O2��]g�A"~N1��]g�A"~M0��]g�A"~L/��]g�A"~K.��]g�A"~J-��]g�A"~I,��]g�A"~H+��]g�A"~G*��]g�A"~F)��]g�A"~E(��]g�A"~D'��]g�A"~C&��]g�A"~B%��]g�A"~A$��]g�A"~@#��]g�A"~?"��]g�A"~>!��]g�A"~= ��]g�A"~<��]g�A"~;��]g�A"~:��]g�A"~9��]g�A"~8��]g�A"~7��]g�A"~6��]g�A"~5��]g�A"~4��]g�A"~3��]g�A"~2��]g�A"~1��]g�A"~0��]g�A"~/��]g�A"~.��]g�A"~-��]g�A"~,��]g�A"~+��]g�A"~*��]g�A"~)��]g�A"~(��]g�A"~'
��]g�A"~&	��]g�A"~%��]    ��]g�A"~� �   ��]g�A"~ � �   ��]g�A"~ �l   ��]g�A"~T7                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 � � � a a                                                                                 CW1 indexsqlite_autoindex_sw_window_settings_1sw_window_settings   �1
33�	tablesw_ui_button_stylessw_ui_button_stylesCREATE TABLE sw_ui_button_styles
(
   id             INTEGER PRIMARY KEY,
   font_id        INTEGER NOT NULL,
   text_color     INTEGER NOT NULL,
   text_scale_x   INTEGER NOT NULL,
   text_scale_y   INTEGER NOT NULL,
   panel_style_id INTEGER NOT NULL
)�	11�itablesw_ui_panel_stylessw_ui_panel_stylesCREATE TABLE sw_ui_panel_styles
(
   id              INTEGER PRIMARY KEY,
   bg_color_top    INTEGER NOT NULL,
   bg_color_bottom INTEGER NOT NULL,
   border_color    INTEGER NOT NULL,
   margin_color    INTEGER NOT NULL,
   margin_left     REAL NOT NULL,
   margin_right    REAL NOT NULL,
   margin_top      REAL NOT NULL,
   margin_bottom   REAL NOT NULL,
   border_left     REAL NOT NULL,
   border_right    REAL NOT NULL,
   border_top      REAL NOT NULL,
   border_bottom   REAL NOT NULL
)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   INTEGER NOT NULL,
   history_index     INTEGER NOT NULL,
   window_mode       INTEGER NOT NULL,
   system_positioned INTEGER NOT NULL,
   maximized         INTEGER NOT NULL,
   save_pos_on_close INTEGER NOT NULL,
   position_x        INTEGER NOT NULL,
   position_y        INTEGER NOT NULL,
   size_x            INTEGER NOT NULL,
   size_y            INTEGER NOT NULL,
   monitor_index     INTEGER NOT NULL,
   refresh_rate      INTEGER NOT NULL,
   v_sync            INTEGER NOT NULL,
   msaa_level        INTEGER NOT NULL,
   red_bits          INTEGER NOT NULL,
   green_bits        INTEGER NOT NULL,
   blue_bits         INTEGER NOT NULL,
   alpha_bits        INTEGER NOT NULL,
   depth_bits        INTEGER NOT NULL,
   stencil_bits      INTEGER NOT NULL,
   srgb_capable      INTEGER NOT NULL,
   use_custom_gamma  INTEGER NOT NULL,
   custom_gamma      REAL NOT NULL,
   PRIMARY KEY (id, history_index)
)                                                                                                                  M�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           �1
33�	tablesw_ui_button_stylessw_ui_button_stylesCREATE TABLE sw_ui_button_styles
(
   id             INTEGER PRIMARY KEY,
   font_id        INTEGER NOT NULL,
   text_color     INTEGER NOT NULL,
   text_scale_x   INTEGER NOT NULL,
   text_scale_y   INTEGER NOT NULL,
   panel_style_id INTEGER NOT NULL
)�u11�tablesw_window_settingssw_window_settingsCREATE TABLE sw_window_settings
(
   id                  CW1 indexsqlite_autoindex_sw_window_settings_1sw_window_settings   be.com/xap/1.0/mm/" xmlns:stRef="http://ns.adobe.com/xap/1.0/sType/ResourceRef#" xmlns:xmp="http://ns.adobe.com/xap/1.0/" xmpMM:OriginalDocumentID="xmp.did:7718DBF4EEE8E211A68ED73740269680" xmpMM:DocumentID="xmp.did:14261CE7E8EF11E28E658058B57530C8" xmpMM:InstanceID="xmp.iid:14261CE6E8EF11E28E658058B57530C8" xmp:CreatorTool="Adobe Photoshop CS5.1 Windows"> <xmpMM:DerivedFrom stRef:instanceID="xmp.iid:7718DBF4EEE8E211A68ED73740269680" stRef:documentID="xmp.did:7718DBF4EEE8E211A68ED73740269680"/> </rdf:Description> </rdf:RDF> </x:xmpmeta> <?xpacket end="r"?>[�   PLTE   ����ٟ�  �IDATx��Zђ�0����ۙ�I�C���} ��.�Ǳe[��
����ױ�qv=^n�(����Lק3\�.'/$�όCv�-�רz�޿���u?�9��>�E���W��h/a�m�w����X�7��L��V�9mMu-D�����4�eX߈o���U�ǟ���^k�_��b�&���[+�3޶������q�j�tA�ϸc�l���7?�U��I����3օ ���v9�3� 0�5l	@��u�Ag𶚿�-�#��+����� ����t؂����n�>�ճg&8�'py���è��e����'�NDC�uA�_�P	TL������p��ѱ�S�	��,�%G��������i ��� ����'ڕ �j M>	�[t_Rh�Ǌ��E�#�(��Xv�K޵�,    @�[��k����R��mƖ�^�r6z �����=e���%���̲�i��s����̓�<�;��V�n���jˢ��J��2?�AA�B�cG���f���ذ{A�p�>�r~0��!����JJ�\ۙ:i�X��]7��e`"�FndQ�%P^2:)|@KK�\��D��+���ə*��1�~*5��G0��a�i��$�Q-4x\��g.d��$�y�j�&�*��K2�Ǟx{������G��g@�ߞ9S�l����`f�vl�}k�iL���i+�g��|�B���d�rns��lچ7��<�'��� ����2�8�&n�b�Y��)�����t�He`���`��z{rh��v#�s�a��,�Wm֫��D�I�^16C$�J3`��F֩f��1	 �tǦ��Juw�Ѳs��U.�+�sB��h�>�R��y(9j(�z�1�11C�82?��_��� �x�4�/�uX��V�\yH��Y��@�a@-�y�iՉ���E߇1�K�腁v`.^�)��S��Y��]�&m<���xˁ%�
֝��
������a�Q�3a������ܺ�������VB�лHE���ȍ�����B>�8���X�n!�:)*��ZV�f!RATq���qJ���쒂v� ;C�nJ��]M �(�߸ڼǝ���T��4�|عbk(�-��|�`jʟ��\[��ݹP?:&�\�М��@�7�6p�pT�%�xэ����ш���X�Dq�[�(@�Ѧz �z.���/�n]0��z��e�K|EPQ�46h�u��q	Q���"���'�"�qw��A��Q��ö́��{W|aQ��g�Y�M�c����G7lԮ󧝄�sv�D���6�����6�z���[9� ���V ���W�i2�K��fa;�]Pŋ��:��w	�=���/�\��7<��;����2��g<��x�3��g<��x�3���  �p%��ð    IEND�B`�   # Z ����{`E*�����oU="�����eJ0 � � � � � t Z            #
��]g�A"~@)	"
��]g�A"~??7!
��]g�A"~>I 
��]g�A"~=*V
��]g�A"~<I
��]g�A"~;1G
		��]g�A"~:W
��]g�A"~9j&
��]g�A"~81
��]g�A"~7b&
��]g�A"~6Z&
��]g�A"~5R&
��]g�A"~4J'
��]g�A"~3B'
��]g�A"~2:'
��]g�A"~14G
��]g�A"~02'
��]g�A"~/5
			��]g�A"~.YX
	��]g�A"~-2Z
	��]g�A"~,nR
��]g�A"~+$P
��]g�A"~**P
��]g�A"~)v	
��]g�A"~(f	

	��]g�A"~'[	
��]g�A"~&"(
��]g�A"~%(	
��]g�A"~$$	
��]g�A"~#a6
��]g�A"~"[
		��]g�A"~!~>
		��]g�A"~ WX

		��]g�A"~QX

��]g�A"~  ����r		   # S ����y^C(�����lQ6 ����z_D) � � � � � n S     F
��]g�A"~c6OE
��]g�A"~bAD
��]g�A"~a<OC
��]g�A"~`[B
		��]g�A"~_[A
��]g�A"~^W@
��]g�A"~]	?
��]g�A"~\?>
��]g�A"~[	=
��]g�A"~Z(8<
��]g�A"~Y 8;
��]g�A"~X8:
��]g�A"~Wl	
9
��]g�A"~V98
��]g�A"~U97
��]g�A"~T96
��]g�A"~S*'5
��]g�A"~Rp.4
��]g�A"~Q(3
��]g�A"~Ph.2
��]g�A"~O`.1
��]g�A"~NX.0
��]g�A"~Mv	/
��]g�A"~LF7.
��]g�A"~KP/-
��]g�A"~JH/,
	��]g�A"~I;G+
��]g�A"~H@/*
��]g�A"~G8/)
��]g�A"~F0/(
��]g�A"~E(0'
��]g�A"~D 0&
��]g�A"~C0%
��]g�A"~B1$
��]g�A"~A1   # Q ����y_E*�����oT9����}cH- � � � � � m Q   i
��]g�A"~ �Ih
		��]g�A"~ �7Gg
��]g�A"~ �N?f
��]g�A"~ �07e
��]g�A"~ �T7d
��]g�A"~ �`>c
		��]g�A"~ �9Gb
		��]g�A"~ �SX
a
��]g�A"~~sR`
��]g�A"~}n	_
		��]g�A"~|~^
��]g�A"~{b	]
��]g�A"~z`M\
��]g�A"~yf>[
��]g�A"~xQZ
��]g�A"~wwFY
��]g�A"~vNNX
��]g�A"~uHNW
��]g�A"~tHV
��]g�A"~sQU
��]g�A"~rWT
��]g�A"~q*@S
��]g�A"~p$@R
��]g�A"~oQQ
��]g�A"~nfLP
��]g�A"~mcF	
O
	��]g�A"~l=GN
��]g�A"~k@M
��]g�A"~j	L
	��]g�A"~i?GK
��]g�A"~h@J
��]g�A"~gAI
��]g�A"~f!HH
��]g�A"~eQG
��]g�A"~dA   ! d ����uZ>#����{_E*�����dG+ � � � � � d                          �

��]g�A"~ �`

�	
��]g�A"~ �T	�
��]g�A"~ �X	
�
��]g�A"~ �h

�
��]g�A"~ �4		�
��]g�A"~ �p

�
��]g�A"~ �x

�
��]g�A"~ �

�
��]g�A"~ �

�
��]g�A"~ �<?� 
��]g�A"~ �	

��]g�A"~ �	
~
��]g�A"~ �	
}
��]g�A"~ �6U|
��]g�A"~ �T?{
��]g�A"~ �}Lz
	��]g�A"~ �WTy
			��]g�A"~ �[Wx
��]g�A"~ �0?w
��]g�A"~ �y6v
	��]g�A"~ �}Ru
��]g�A"~ �Wt
��]g�A"~ �xLs
��]g�A"~ �GGr
��]g�A"~ �STq
	��]g�A"~ �AYp
��]g�A"~ �x.o
	��]g�A"~ �-Zn
��]g�A"~ �0Vm
��]g�A"~ �Vl
��]g�A"~ �6?k
��]g�A"~ �r&j
		��]g�A"~ �IX   ! L ����pR4�����nQ4�����mP3 � � � � � i L  �+
��]g�A"~ �m�*
��]g�A"~ �g�)
��]g�A"~ �A�(
��]g�A"~ �YG	
�'
��]g�A"~ �a�&
��]g�A"~ �B?�%
��]g�A"~ �[�$
��]g�A"~ �U�#
��]g�A"~ �O�"
��]g�A"~ �I�!
��]g�A"~ �0� 
��]g�A"~ �C�
��]g�A"~ �8

�
��]g�A"~ �D		�
��]g�A"~ �

�
��]g�A"~ �0

�
��]g�A"~ �@

�
��]g�A"~ �H?�
��]g�A"~ �BN�
��]g�A"~ �,		�
��]g�A"~ �=�
��]g�A"~ �H

�
��]g�A"~ � 

�
��]g�A"~ �

�
��]g�A"~ �

�
��]g�A"~ �	)	�
��]g�A"~ �j	�	�
��]g�A"~ �
�
�
��]g�A"~ �

�
��]g�A"~ �!
�
�
��]g�A"~ �L		�
��]g�A"~ �P

�
��]g�A"~ �X

     a ����mO2�����hK.�����dG* � � � � ~ a                         �K
��]g�A"~~*�J
��]g�A"~}

�I
��]g�A"~x<		�H
��]g�A"~a:�G
��]g�A"~`(

�F
��]g�A"~SmF	
�E
��]g�A"~Rb	
�D
��]g�A"~BH�C
��]g�A"~A(�B
	��]g�A"~1qR�A
��]g�A"~ �\	�@
��]g�A"~ �7�?
��]g�A"~ �
�>
��]g�A"~ �Z?�=
��]g�A"~ �$�<
��]g�A"~ ��;
��]g�A"~ ��:
��]g�A"~ �P�9
��]g�A"~ �0P�8
��]g�A"~ �m6�7
��]g�A"~ ��6
��]g�A"~ ��5
��]g�A"~ � �4
��]g�A"~ � �3
��]g�A"~ �y�2
��]g�A"~ �g6�1
��]g�A"~ �I��0
��]g�A"~ �D��/
��]g�A"~ �K�.
��]g�A"~ �H��-
��]g�A"~ �s6�,
��]g�A"~ �s   ! N ����pU8�����nR6�����nQ4 � � � � � k N    �l
��]g�A"~!&87�k
��]g�A"~!"W	�j
��]g�A"~ D0�i
��]g�A"~ :_S�h
��]g�A"~ 9hR�g
��]g�A"~ 0N	
�f
		��]g�A"~ &"Z�e
��]g�A"~ "[S�d
��]g�A"~ !%H�c
��]g�A"~  	W�b
	��]g�A"~ FT�a
��]g�A"~ AU�`
��]g�A"~ <U�_
	��]g�A"~ kR�^
��]g�A"~ eS�]
��]g�A"~ bS�\
	��]g�A"~ 7Y�[
	��]g�A"~ MX�Z
	��]g�A"~ <Y�Y
��]g�A"~�Q�X
��]g�A"~�l>�W
��]g�A"~�M7�V
��]g�A"~�AG�U
��]g�A"~�]�T
��]g�A"~�]�S
��]g�A"~�KT�R
��]g�A"~�OT�Q
			��]g�A"~�UX�P
	��]g�A"~�xR�O
	��]g�A"~�EY�N
	��]g�A"~�
]�M
	��]g�A"~�\�L
	��]g�A"~�z	    ����qV9�����nQ3                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           �}
��]g�A"~ �r>�|
��]g�A"~ �[6�{
��]g�A"~%�rL�z
��]g�A"~"�lL�y
��]g�A"~"e-H�x
��]g�A"~"d)H�w
��]g�A"~"`ZM�v
��]g�A"~"HTN�u
��]g�A"~"+
�t
��]g�A"~"$V�s
��]g�A"~"SG�r
			��]g�A"~"]W�q
	��]g�A"~"(Z�p
��]g�A"~"z&�o
��]g�A"~"I�n
��]g�A"~"MG�m
��]g�A"~"x>
   5 �  � �!0?N]l{�������� />M\kz��������.=L[jy��������������              ��]g�A"~��]g�A"~ ��]g�A"~!��]g�A"~"��]g�A"~#��]g�A"~$��]g�A"~��]g�A"~ ��]g�A"~!��]g�A"~"��]g�A"~#��]g�A"~$��]g�A"~%��]g�A"~&	��]g�A"~'
��]g�A"~(��]g�A"~)��]g�A"~*��]g�A"~+��]g�A"~,��]g�A"~-��]g�A"~.��]g�A"~/��]g�A"~0��]g�A"~1��]g�A"~2��]g�A"~3��]g�A"~4��]g�A"~5��]g�A"~6��]g�A"~7��]g�A"~8��]g�A"~9��]g�A"~:��]g�A"~;��]g�A"~<��]g�A"~= ��]g�A"~>!��]g�A"~?"��]g�A"~@#��]g�A"~A$��]g�A"~B%��]g�A"~C&��]g�A"~D'��]g�A"~E(��]g�A"~F)��]g�A"~G*��]g�A"~H+��]g�A"~I,��]g�A"~J-��]g�A"~K.��]g�A"~L/��]g�A"~M0��]g�A"~N1��]g�A"~O2��]g�A"~P3��]g�A"~Q4��]g�A"~R5��]g�A"~S6
   4 �  � �&5DSbq���������%4CRap��������$3BQ`p�������������� �    ���]g�A"~U8��]g�A"~V9��]g�A"~W:��]g�A"~X;��]g�A"~Y<��]g�A"~Z=���]g�A"~U8��]g�A"~V9��]g�A"~W:��]g�A"~X;��]g�A"~Y<��]g�A"~Z=��]g�A"~[>��]g�A"~\?��]g�A"~]@��]g�A"~^A��]g�A"~_B��]g�A"~`C��]g�A"~aD��]g�A"~bE��]g�A"~cF��]g�A"~dG��]g�A"~eH��]g�A"~fI��]g�A"~gJ��]g�A"~hK��]g�A"~iL��]g�A"~jM��]g�A"~kN��]g�A"~lO��]g�A"~mP��]g�A"~nQ��]g�A"~oR��]g�A"~pS��]g�A"~qT��]g�A"~rU��]g�A"~sV��]g�A"~tW��]g�A"~uX��]g�A"~vY��]g�A"~wZ��]g�A"~x[��]g�A"~y\��]g�A"~z]��]g�A"~{^��]g�A"~|_��]g�A"~}`��]g�A"~~a��]g�A"~ �b��]g�A"~ �c��]g�A"~ �d��]g�A"~ �e��]g�A"~ �f��]g�A"~ �g��]g�A"~ �h��]g�A"~ �i��]g�A"~ �j��]g�A"~ �k
   2 �  � � � �!1AQaq��������$5FWhy�������#4EVgx����������� � ��]g�A"~ �s��]g�A"~ �t��]g�A"~ �u��]g�A"~ �v��]g���]g�A"~ �m��]g�A"~ �n��]g�A"~ �o��]g�A"~ �p��]g�A"~ �q��]g�A"~ �r��]g�A"~ �s��]g�A"~ �t��]g�A"~ �u��]g�A"~ �v��]g�A"~ �w��]g�A"~ �x��]g�A"~ �y��]g�A"~ �z��]g�A"~ �{��]g�A"~ �|��]g�A"~ �}��]g�A"~ �~��]g�A"~ ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � �
   0 �  � � �%6GXiz�������$5FWhy�������#4EVgx������������         ��]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~ � ���]g�A"~1 ���]g�A"~A ���]g�A"~B ���]g�A"~R ���]g�A"~S ���]g�A"~` ���]g�A"~a ���]g�A"~x ���]g�A"~} ���]g�A"~~ ���]g�A"~� ���]g�A"~� ���]g�A"~� ���]g�A"~� �
   . � %6GXiz�������$5FWhy�������#4EVgx������� � � � � � � � {         ��]g�A"~"� ���]g�A"~"e ���]g�A"~"d ���]g�A"~"` ���]g�A"~"H ���]g�A"~"+ ���]g�A"~	��]g�A"~  ������]g�A"~ � ���]g�A"~ � ���]g�A"~� ���]g�A"~� ���]g�A"~� ���]g�A"~� ���]g�A"~� ���]g�A"~� ���]g�A"~� ���]g�A"~� ���]g�A"~� ���]g�A"~  ���]g�A"~  ���]g�A"~  ���]g�A"~  ���]g�A"~  ���]g�A"~  ���]g�A"~  ���]g�A"~  ���]g�A"~  ���]g�A"~   ���]g�A"~ ! ���]g�A"~ " ���]g�A"~ & ���]g�A"~ 0 ���]g�A"~ 9 ���]g�A"~ : ���]g�A"~ D ���]g�A"~!" ���]g�A"~!& ���]g�A"~" ���]g�A"~" ���]g�A"~" ���]g�A"~" ���]g�A"~" ���]g�A"~" ���]g�A"~" ���]g�A"~" ���]g�A"~"+ ���]g�A"~"H ���]g�A"~"` ���]g�A"~"d ���]g�A"~"e ���]g�A"~"� ���]g�A"~%� �