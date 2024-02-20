# RO:CO
RO:CO is a open-source program which specializes in compressing .fbx files using the LZMA compression algorithm which was originally created by 7zip. Further tweaking to LZMA settings will be required in order to compress .fbx files, this will require further research in how the .fbx storage type works in order to achieve the most efficient and effective compression. I will most probably compile a research paper on my findings and how the program settings will have needed to adapt to these conditions.

This program is designed to help all 3D artists reduce the file sizes of their project files without losing any model, scene or animation detail.

Very experimental version, no GUI only CLI.

### Features:
- Compress .fbx into .lzma
- Decompress .lzma to .fbx
- Compress .obj into .lzma
- Decompress .lzma to .obj


### Compression Comparison (OBJ)
- Original: 202mb
- ZIP Compression: 60.8mb 
- RAR Compression: 58.9mb
- CO-OBJ LZMA: 46.4mb

### Compression Comparison (FBX)
- Original: 12.1mb
- ZIP Compression: 11.2mb (8% less)
- CO-FBX LZMA: 10.8mb (11% less)



These may seem very minor improvements but changes will be made and more file-types will be supported.
