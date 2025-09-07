

/////////////////////////////////////////////////////////////
// Google Fonts support (requires internet connection and font download)
// Note: This downloads fonts to SD card for caching
/////////////////////////////////////////////////////////////
inline const lv_font_t* chinScreen_load_google_font(const char* fontName, int size = 16) {
    // Create filename for cached font
    char filename[64];
    snprintf(filename, sizeof(filename), "/fonts/%s_%d.bin", fontName, size);
    
    // Check if font exists in cache
    if (SD.exists(filename)) {
        // Load from SD card cache
        File fontFile = SD.open(filename, FILE_READ);
        if (fontFile) {
            size_t fontSize = fontFile.size();
            uint8_t* fontData = (uint8_t*)malloc(fontSize);
            if (fontData) {
                fontFile.read(fontData, fontSize);
                fontFile.close();
                
                // Create LVGL font descriptor
                // Note: This is simplified - actual implementation would need proper font parsing
                static lv_font_t customFont;
                // You would need to properly parse the font data here
                return &customFont;
            }
        }
    }
    
    // Font not cached - would need to download from Google Fonts API
    // This requires WiFi connection and HTTP client
    Serial.printf("Font %s size %d not found in cache\n", fontName, size);
    return FONT_MEDIUM; // Return default font
}


