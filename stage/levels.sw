SQLite format 3   @     c              
                                                 c -�)   �    ����                                                                                             �++�ktablesw_map_entitiessw_map_entitiesCREATE TABLE sw_map_entities
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
   � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                	id   � ��                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                ���·�� test2���·��� test1   �    �����S��zB � � p 8            6	�|HM�2����   @6�    Ig���)k�/͓)�=��6	�|HM�2��D�ـ   ?�6(    Ig���)k�/͓)�=��6	�|HM�2��Hr��   ����   Ig���)k�/͓)�=��6	�|HM�2��E4�    �S�    Ig���)k�/͓)�=��(		�|HM�2�-�
Ig���)k�?���-f�6	�|HM�2��5��`   @1J�   Ig���)k�/͓)�=��(	�|HM�2�
Ig���)k�p�.����z6	�|HM�2��"2A    @")    Ig���)k�/͓)�=��=		�|HM�2�@���   @���   ��T�   PIg���)k�?���-f�6	�|HM�2��4�]�   @
    Ig���)k�/͓)�=��6	�|HM�2�	@4l �   @1|J�   Ig���)k���^���6
	�|HM�2��A���   ����   Ig���)k���^���6		�|HM�2�@A7@   @�Y    Ig���)k���^���6	�|HM�2�@+L��   @�	    Ig���)k���^���'	�HM�8
#Ig���)k�?���-f�'		�HM�8��(Ig���)k�?���-f�   		6	�|HM�2��nS    ?��    Ig���)k���^���6	�|HM�2��2��    ����   Ig���)   &   
� (� _A#�����|�mP2�������~o`QB3$���������                                                                                                                                                                                                                                                                                                                                     �|HM�2�*+�|HM�2�)*�|HM�2�()�|HM�2�'(�|HM�2�&'�|HM�2�%&�|HM�2�$%�|HM�2�#$�|HM�2�"#�|HM�2�!"�|HM�2� !�|HM�2� �|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2��|HM�2�	�|HM�2��|HM�2��|HM�2�	�|HM�2�
�|HM�2�	�|HM�2��HM�8	�HM�8   �|HM�2��|HM�2��|HM�2�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  �  �o�$��                                                                                             �                                                                                               ��99�Atablesw_sandwich_propertiessw_sandwich_propertiesCREATE TABLE sw_sandwich_properties
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
)                                                                                                              �  8 \3�����S��zB � � p 8            6	�|HM�2����   @6�    Ig���)k�/͓)�=��6	�|HM�2��D�ـ   ?�6(    Ig���)k�/͓)�=��6	�|HM�2��Hr��   ����   Ig���)k�/͓)�=��6	�|HM�2��E4�    �S�    Ig���)k�/͓)�=��(		�|HM�2�-�
Ig���)k�?���-f�6	�|HM�2��5��`   @1J�   Ig���)k�/͓)�=��(	�|HM�2�
Ig���)k�p�.����z6	�|HM�2��"2A    @")    Ig���)k�/͓)�=��=		�|HM�2�@���   @���   ��T�   PIg���)k�?���-f�6	�|HM�2��4�]�   @
    Ig���)k�/͓)�=��6	�|HM�2�	@4l �   @1|J�   Ig���)k���^���6
	�|HM�2��A���   ����   Ig���)k���^���6		�|HM�2�@A7@   @�Y    Ig���)k���^���6	�|HM�2�@+L��   @�	    Ig���)k���^���'	�HM�8
#Ig���)k�?���-f�'		�HM�8��(Ig���)k�?���-f�   		6	�|HM�2��nS    ?��    Ig���)k���^���6	�|HM�2��2��    ����   Ig���)k���^���    H ��X ��x@��`( � � � H                              6&	�|HM�2�%@)���   @F&�    Ig���)k�vМM�D�6%	�|HM�2�$@<��`   @E"M`   Ig���)k�vМM�D�6$	�|HM�2�#@Bn�   @Fq`   Ig���)k�vМM�D�6#	�|HM�2�"@7��    @E�`   Ig���)k�vМM�D�6"	�|HM�2�!@I�b�   @H�    Ig���)k�쭏S����6!	�|HM�2� @D��   @H?�    Ig���)k�쭏S����6 	�|HM�2�@O��@   @Ech    Ig���)k�쭏S����6	�|HM�2�@Mw�   @�    Ig���)k�쭏S����6	�|HM�2�@P��   @�*    Ig���)k�쭏S����6	�|HM�2�@PZ�@   @3@�@   Ig���)k�쭏S����6	�|HM�2�@O��   @=΀    Ig���)k�쭏S����6	�|HM�2�@I�    @Bfn�   Ig���)k�쭏S����6	�|HM�2�@BȾ�   @@�A`   Ig���)k�쭏S����6	�|HM�2�@ID�   @7�r�   Ig���)k�쭏S����6	�|HM�2�@G},    ?�P    Ig���)k�쭏S����6	�|HM�2�@LR&�   @.^    Ig���)k�쭏S����6	�|HM�2�@�i`   @)G�    Ig���)k�/͓)�=��   � ��X �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      6+	�|HM�2�*?�K&    @E�<    Ig���)k�vМM�D�6*	�|HM�2�)@@&�   @A�    Ig���)k�vМM�D�6)	�|HM�2�(@9޲    @CJL    Ig���)k�vМM�D�6(	�|HM�2�'@5k_�   @D��    Ig���)k�vМM�D�6'	�|HM�2�&@%y�@   @D�    Ig���)k�vМM�D�