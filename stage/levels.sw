SQLite format 3   @    �            
                                                � -�)   �    ����                                                                                             �++�ktablesw_map_entitiessw_map_entitiesCREATE TABLE sw_map_entities
(
   map_id      INTEGER NOT NULL,
   entity_id   INTEGER NOT NULL,
   entity_type INTEGER NOT NULL,
   rotation    REAL NOT NULL,
   pos_x       REAL NOT NULL,
   pos_y       REAL NOT NULL,
   scale_x     REAL NOT NULL,
   scale_y     REAL NOT NULL,
   material_sw_id INTEGER,
   material_id INTEGER,
   PRIMARY KEY (map_id, entity_id)
)=Q+ indexsqlite_autoindex_sw_map_entities_1sw_map_entitiesf�#tablesw_mapssw_mapsCREATE TABLE sw_maps
(
   id   INTEGER PRIMARY KEY,
   name TEXT NOT NULL
)�99�Atablesw_sandwich_propertiessw_sandwich_propertiesCREATE TABLE sw_sandwich_properties
(
   property TEXT PRIMARY KEY, 
   value    NUMERIC
)K_9 indexsqlite_autoindex_sw_sandwich_properties_1sw_sandwich_      	      � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       id-M�	�s�j
   � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                	id   � ��                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   �·�����·�� ���·���    �    ���'_����g/ � � � H .  6	�|HM�2�@ID�6:	�|HM�2�
�D�ـ   ?�6(    Ig���)k�/͓)�=��=9		�|HM�2�@���   @���   ��T�   PIg���)k�����sq�l68	�|HM�2�	�Hr��   ����   Ig���)k�/͓)�=��67	�|HM�2��+�    @ oZ�   Ig���)k���^���66	�|HM�2�@4l �   @1|J�   Ig���)k���^���65	�|HM�2��3�~�   @"|��   Ig���)k���^���64	�|HM�2�@,x��   @}��   Ig���)k���^���63	�|HM�2�%?ҿ��   @     Ig���)k���^���6*	�HM�8?���   @0��@   Ig���)k���^���6+	�HM�8�:%��   @']r    Ig���)k���^���6,	�HM�8�0>U`   @U`   Ig���)k���^���6-	�HM�8@5��   @*U`   Ig���)k���^���6.	�HM�8@'�U�   @+�     Ig���)k���^���6/	�HM�8�	U�   @-�     Ig���)k���^���60	�HM�8	�@��    @#�U�   Ig���)k���^���61		�HM�8��<�     ?�V�   (Ig���)k�?���-f�/2	�HM�8
��      #Ig���)   L   :
   -_ {]N���?�l�0!��������|m^O@1"������_�����}n������ � � �^2��  �|HM�2�6>�|HM�2�5=�|HM�2�4<�|HM�2�3;�|HM�2�2:�|HM�2�19�|HM�2�08�|HM�2�/7�|HM�2   �|HM�2�(0   �|HM�2�)1   �|HM�2�*2   �|HM�2�+3   �|HM�2�,4   �|HM�2�-5 �HM�82�HM�8	0�HM�8/�HM�8.�HM�8-�HM�8,�HM�8+�|HM�2�$V�|HM�2�#U�|HM�2�"T�|HM�2�!S�|HM�2� R�|HM�2�Q�|HM�2�P�|HM�2�O�|HM�2�N�|HM�2�M�|HM�2�L�|HM�2�K�|HM�2�J�|HM�2�I�|HM�2�H�|HM�2�G�|HM�2�F�|HM�2�E�|HM�2�D�|HM�2�C�|HM�2�B�|HM�2�A�|HM�2�@�|HM�2�?�|HM�2�>�|HM�2�=�|HM�2�<�|HM�2�;�|HM�2�
:	�|HM�2�9�|HM�2�	8�|HM�2�7�|HM�2�6�|HM�2�5�|HM�2�4�|HM�2�%3	�HM�81�HM�8*                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  �  �o�$��                                                                                             �                                                                                               ��99�Atablesw_sandwich_propertiessw_sandwich_propertiesCREATE TABLE sw_sandwich_properties
(
   property TEXT PRIMARY KEY, 
   value    NUMERIC
)K_9 indexsqlite_autoindex_sw_sandwich_properties_1sw_sandwich_propertiesf�#tablesw_mapssw_mapsCREATE TABLE sw_maps
(
   id   INTEGER PRIMARY KEY,
   name TEXT NOT NULL
)�++�ktablesw_map_entitiessw_map_entitiesCREATE TABLE sw_map_entities
(
   map_id      INTEGER NOT NULL,
   entity_id   INTEGER NOT NULL,
   entity_type INTEGER NOT NULL,
   rotation    REAL NOT NULL,
   pos_x       REAL NOT NULL,
   pos_y       REAL NOT NULL,
   scale_x     REAL NOT NULL,
   scale_y     REAL NOT NULL,
   material_sw_id INTEGER,
   material_id INTEGER,
   PRIMARY KEY (map_id, entity_id)
)=Q+ indexsqlite_autoindex_sw_map_entities_1sw_map_entities    �  ��e���77�Otablesw_texture_font_charssw_texture_font_charsCREAT�##�utablesw_texturessw_texturesCREATE TABLE sw_textures
(
   id         �##�utablesw_texturessw_texturesCREATE TABLE sw_textures
(
   id              INTEGER PRIMARY KEY,
   data            NOT NULL,
   internal_format INTEGER NOT NULL,
   srgb            INTEGER NOT NULL,
   mag_filter      INTEGER NOT NULL,
   min_filter      INTEGER NOT NULL
)�(--�tablesw_texture_fontssw_texture_fonts
CREATE TABLE sw_texture_fonts
(
   id         INTEGER PRIMARY KEY,
   texture_id NOT NULL,
   cap_height INTEGER NOT NULL
)�77�Otablesw_texture_font_charssw_texture_font_charsCREATE TABLE sw_texture_font_chars
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
)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                �  �� �                                                                                                                                                                                            I	]7 indexsqlite_autoindex_sw_texture_font_chars_1sw_texture_font_chars�C
%%�Itablesw_materialssw_materialsCREATE TABLE sw_materials
(
   id           INTEGER PRIMARY KEY,
   color        INTEGER NOT NULL,
   texture_id   INTEGER,
   texture_mode INTEGER NOT NULL
)�11�itablesw_ui_panel_stylessw_ui_panel_stylesCREATE TABLE sw_ui_panel_styles
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
)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            �  ��                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       �u11�tablesw_window_settingssw_window_settingsCREATE TABLE sw_window_settings
(
   id                  CW1 indexsqlite_autoindex_sw_window_settings_1sw_window_settings   �133�	tablesw_ui_button_stylessw_ui_button_stylesCREATE TABLE sw_ui_button_styles
(
   id             INTEGER PRIMARY KEY,
   font_id        INTEGER NOT NULL,
   text_color     INTEGER NOT NULL,
   text_scale_x   INTEGER NOT NULL,
   text_scale_y   INTEGER NOT NULL,
   panel_style_id INTEGER NOT NULL
)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
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
)                                                                                                                  H G��'_����g/ � � � H                              6:	�|HM�2�
�D�ـ   ?�6(    Ig���)k�/͓)�=��=9		�|HM�2�@���   @���   ��T�   PIg���)k�����sq�l68	�|HM�2�	�Hr��   ����   Ig���)k�/͓)�=��67	�|HM�2��+�    @ oZ�   Ig���)k���^���66	�|HM�2�@4l �   @1|J�   Ig���)k���^���65	�|HM�2��3�~�   @"|��   Ig���)k���^���64	�|HM�2�@,x��   @}��   Ig���)k���^���63	�|HM�2�%?ҿ��   @     Ig���)k���^���6*	�HM�8?���   @0��@   Ig���)k���^���6+	�HM�8�:%��   @']r    Ig���)k���^���6,	�HM�8�0>U`   @U`   Ig���)k���^���6-	�HM�8@5��   @*U`   Ig���)k���^���6.	�HM�8@'�U�   @+�     Ig���)k���^���6/	�HM�8�	U�   @-�     Ig���)k���^���60	�HM�8	�@��    @#�U�   Ig���)k���^���61		�HM�8��<�     ?�V�   (Ig���)k�?���-f�/2	�HM�8
��      #Ig���)k�?���-f�   
� ��X ��x@�                                                                                                                                                                                                                                                                                                                                                                                                                                                    6V	�|HM�2�$@B��    @)��    Ig���)k�/͕)�=�G6U	�|HM�2�#@��`   @5�    Ig���)k�w�Ѩ��m6T	�|HM�2�"?�K&    @E�<    Ig���)k�vМM�D�6S	�|HM�2�!@@&�   @A�    Ig���)k�vМM�D�6R	�|HM�2� @9޲    @CJL    Ig���)k�vМM�D�6Q	�|HM�2�@5k_�   @D��    Ig���)k�vМM�D�6P	�|HM�2�@%y�@   @D�    Ig���)k�vМM�D�6O	�|HM�2�@)���   @F&�    Ig���)k�vМM�D�6N	�|HM�2�@<��`   @E"M`   Ig���)k�vМM�D�6M	�|HM�2�@Bn�   @Fq`   Ig���)k�vМM�D�v  5 ��&^��>��~F � � f 5           /2	�HM�8
��      #Ig���)k�?���-f�61		�HM�8��<�     ?�V�   (Ig���)k�?���-f�60	�HM�8	�@��    @#�U�   Ig���)k���^���6/	�HM�8�	U�   @-�     Ig���)k���^���6.	�HM�8@'�U�   @+�     Ig���)k���^���6-	�HM�8@5��   @*U`   Ig���)k���^���6,	�HM�8�0>U`   @U`   Ig���)k���^���6"	�|HM�2�!@)���   @F&�    Ig���)k�vМM�D�6#	�|HM�2�"@%y�@   @D�    Ig���)k�vМM�D�6$	�|HM�2�#@5k_�   @D��    Ig���)k�vМM�D�6%	�|HM�2�$@9޲    @CJL    Ig���)k�vМM�D�6&	�|HM�2�%@@&�   @A�    Ig���)k�vМM�D�6'	�|HM�2�&?�K&    @E�<    Ig���)k�vМM�D�6(	�|HM�2�(@��`   @5�    Ig���)k�w�Ѩ��m6)	�|HM�2�O@B��    @)��    Ig���)k�/͕)�=�G   	�HM�8�:%6+	�HM�8�:%��   @']r    Ig���)k���^���6*	�HM�8?���   @0��@   Ig���)k���^���                             � x @                      6&	�|HM�2�@7��    @E�`   Ig���)k�vМM�D�6%	�|HM�2�@I�b�   @H�    Ig���)k�쭏S����6$	�|HM�2�@D��   @H?�    Ig���)k�쭏S����6#	�|HM�2�@O��@   @Ech    Ig���)k�쭏S����6"	�|HM�2�@Mw�   @�    Ig���)k�쭏S����6!	�|HM�2�@P��   @�*    Ig���)k�쭏S����6 	�|HM�2�@PZ�@   @3@�@   Ig���)k�쭏S����66.	�|HM�2�&?�K&    @E�<    Ig���)k�vМM�D�L/	�|HM�2�'�%{��   �&a*�   @$�    @"8t�   ?����   Ig���)k��h�)����60	�|HM�2�(@��`   @5�    Ig���)k�w�Ѩ��m61	�|HM�2�)�3o @   @���   Ig���)k���#�"���>2	�|HM�2�*�)���   �-���   @\     Ig���)k��I�h��>3	�|HM�2�+@5| @   �� �   @qU@   Ig���)k�V.-�S�@�>4	�|HM�2�,�8���   ?�EU@   @#U�   Ig���)k��O�h�>5	�|HM�2�-@0�    @#���   @.gU`   Ig���)k�V.+�S�=6>6	�|HM�2�.�@ �   @'� �   @ �     Ig���)k�{��0�8�
   -^ ^l{�������� />M\kz��������.=L[jy�������� � � � � � � � � �^2��  �|HM�2�6>�|HM�2�5=�|HM�2�4<�|HM�2�3;�|HM�2�2:�|HM�2�19�|HM�2�08�|HM�2�/7�|HM�2�.6�|HM�2�-5�|HM�2�,4�|HM�2�+3�|HM�2�*2�|HM�2�)1�|HM�2	�|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2�	�|HM�2�
�|HM�2��|HM�2�	�|HM�2�
�|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2� �|HM�2�!�|HM�2�"�|HM�2�#�|HM�2�$�|HM�2�%�|HM�2�&�|HM�2�'�|HM�2� (�|HM�2�!)�|HM�2�"*�|HM�2�#+�|HM�2�$,�|HM�2�%-�|HM�2�&.�|HM�2�'/�|HM�2�(0�|HM�2�)1�|HM�2�*2�|HM�2�+3�|HM�2�,4�|HM�2�-5
   /@ ^m|��������!0?N]l{�������� />M\kz������O@�� � � � � � � � ���              �|HM�2�X]�|HM�2�W\�|HM�2�V[�|HM�2�UZ�|HM�2�TY�|HM�2�SX�|HM�2�RW�|HM�2�QV�|HM�2�PU�|HM�2�OT�|HM�2�NS�|HM�2�MR�|HM�2�[`�|HM�2�Z_�|HM�2�/7�|HM�2�08�|HM�2�19�|HM�2�2:�|HM�2�3;�|HM�2�4<�|HM�2�5=�|HM�2�6>�|HM�2�7?�|HM�2�8@�|HM�2�9A�|HM�2�:B�|HM�2�;C�|HM�2�<D�|HM�2�=E�|HM�2�>F�|HM�2�?G�|HM�2�@H�|HM�2�A�|HM�2�B�|HM�2�C�|HM�2�DI�|HM�2�EJ�|HM�2�FK�|HM�2�GL�|HM�2�HM�|HM�2�IN�|HM�2�JO�|HM�2�KP�|HM�2�LQ�|HM�2�MR�|HM�2�NS�|HM�2�OT�|HM�2�PU�|HM�2�QV�|HM�2�RW�|HM�2�SX�|HM�2�TY�|HM�2�UZ�|HM�2�V[�|HM�2�W\�|HM�2�X]�|HM�2�Y^	�HM�8�HM�8    H ��X ��x@��`( � � � H                              6W	�|HM�2�R@J`   �#tx    Ig���)k�/͕)�=�G6V	�|HM�2�Q��w?�   �'�=�   Ig���)k�/͕)�=�G6U	�|HM�2�P@>�`   �!��    Ig���)k�/͕)�=�G6T	�|HM�2�O@B��    @)��    Ig���)k�/͕)�=�G6S	�|HM�2�N@HÄ�   @(��`   Ig���)k�/͕)�=�G6R	�|HM�2�M@J�`   ����   Ig���)k�/͕)�=�G6Q	�|HM�2�L@D�    ��`    Ig���)k�/͕)�=�G6P	�|HM�2�K@<J�    ���    Ig���)k�/͕)�=�G6O	�|HM�2�J@C���   ?�j     Ig���)k�/͕)�=�G6N	�|HM�2�I@Bx0�   �$���   Ig���)k�/͕)�=�G6M	�|HM�2�H@5�!    �!���   Ig���)k�/͕)�=�G6L	�|HM�2�G@2��    �I�   Ig���)k�/͕)�=�G6K	�|HM�2�F@5Ly    �;@   Ig���)k�/͕)�=�G6J	�|HM�2�E@.c�@   ��I�   Ig���)k�/͕)�=�G6I	�|HM�2�D@(��    �".�   Ig���)k�/͕)�=�G6H	�|HM�2�@@ &    @'�=`   Ig���)k�/͕)�=�G6G	�|HM�2�?@!k��   @,�S    Ig���)k�/͕)�=�G    , ��t<��\$��|D � � d ,6L	�|HM�2�@7��    @E�`   Ig���)k�vМM�D�6K	�|HM�2�@I�b�   @H�    Ig���)k�쭏S����6J	�|HM�2�@D��   @H?�    Ig���)k�쭏S����6I	�|HM�2�@O��@   @Ech    Ig���)k�쭏S����6H	�|HM�2�@Mw�   @�    Ig���)k�쭏S����6G	�|HM�2�@P��   @�*    Ig���)k�쭏S����6F	�|HM�2�@PZ�@   @3@�@   Ig���)k�쭏S����6E	�|HM�2�@O��   @=΀    Ig���)k�쭏S����6D	�|HM�2�@I�    @Bfn�   Ig���)k�쭏S����6C	�|HM�2�@BȾ�   @@�A`   Ig���)k�쭏S����6B	�|HM�2�@ID�   @7�r�   Ig���)k�쭏S����6A	�|HM�2�@G},    ?�P    Ig���)k�쭏S����6@	�|HM�2�@LR&�   @.^    Ig���)k�쭏S����6?	�|HM�2����   @6�    Ig���)k�/͓)�=��6>	�|HM�2��5��`   @1J�   Ig���)k�/͓)�=��6=	�|HM�2��E4�    �S�    Ig���)k�/͓)�=��(<		�|HM�2�-�
Ig���)k�����sq�l(;	�|HM�2�
Ig���)k��8Y��J