CREATE TABLE sw_sandwich_properties
(
      property          TEXT PRIMARY KEY,
      value             NUMERIC
);

CREATE TABLE sw_maps
(
      id                INTEGER PRIMARY KEY,
      name              TEXT NOT NULL,
);

CREATE TABLE sw_map_entities
(
      map_id            INTEGER NOT NULL,
      id                INTEGER PRIMARY KEY,
      entity_type       INTEGER NOT NULL,
      r_x               REAL NOT NULL,
      r_y               REAL NOT NULL,
      r_z               REAL NOT NULL,
      r_w               REAL NOT NULL,
      t_x               REAL NOT NULL,
      t_y               REAL NOT NULL,
      t_z               REAL NOT NULL,
      s_x               REAL NOT NULL,
      s_y               REAL NOT NULL,
      s_z               REAL NOT NULL,
      mesh_id           INTEGER,
      program_id        INTEGER,
      collider_id       INTEGER
);

CREATE TABLE sw_textures
(
      -- Identifies this texture among all other textures in this sandwich
      id                INTEGER PRIMARY KEY,
      -- The image data for the texture.  Format depends on the value of `data_format`
      data              NOT NULL,
      -- Describes the OpenGL internal format to request for storing the texture on the GPU.
      internal_format   INTEGER NOT NULL,
      -- If non-zero, the texture data will be linearized from sRGB when sampling.
      srgb              INTEGER NOT NULL,
      -- The sampler filtering mode to use for magnification
      mag_filter        INTEGER NOT NULL,
      -- The sampler filtering mode to use for minification
      min_filter        INTEGER NOT NULL
);

CREATE TABLE sw_texture_fonts
(
      id                INTEGER PRIMARY KEY,
      texture_id        INTEGER NOT NULL,
      cap_height        INTEGER NOT NULL
);

CREATE TABLE sw_texture_font_chars
(
      font_id           INTEGER NOT NULL,
      codepoint         INTEGER NOT NULL,
      
      tc_x              INTEGER NOT NULL,
      tc_y              INTEGER NOT NULL,
      
      tc_width          INTEGER NOT NULL,
      tc_height         INTEGER NOT NULL,
      
      offset_x          INTEGER NOT NULL,
      offset_y          INTEGER NOT NULL,
      
      advance           INTEGER NOT NULL,
      
      PRIMARY KEY (font_id, codepoint)
);

CREATE TABLE sw_window_settings
(
      -- Identifies this set of window settings, allowing multiple windows to
      -- store their settings independently.
      id                INTEGER NOT NULL,
      -- When window settings are changed (except when saving new window
      -- position/size on window close), a new history index is created so
      -- that the old settings can be restored if the new ones are bad.
      history_index     INTEGER NOT NULL,
      
      -- Determines whether the window is fullscreen or not, and how much
      -- control the user has over its position/size.
      --    0  Windowed, resizable
      --    1  Windowed, not resizable
      --    2  Windowed, undecorated, resizable
      --    3  Windowed, undecorated, not resizable
      --    7  Windowed Fullscreen
      --    8  Exclusive Fullscreen
      window_mode       INTEGER NOT NULL,
      -- If 0, the window should be moved to `position_x`,`position_y` after it
      -- is created.  Ignored in fullscreen exclusive mode.
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
);
