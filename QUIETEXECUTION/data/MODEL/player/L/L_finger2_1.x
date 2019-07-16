xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 291;
 0.68678;-1.67240;-0.00637;,
 0.34720;-1.77542;-0.00637;,
 0.34042;-1.77542;-0.07527;,
 0.67347;-1.67240;-0.14152;,
 0.99974;1.50512;-0.00637;,
 1.27405;1.28000;-0.00637;,
 1.24946;1.28000;-0.25609;,
 0.98042;1.50512;-0.20257;,
 1.49917;1.00569;-0.00637;,
 1.66646;0.69273;-0.00637;,
 1.63432;0.69273;-0.33264;,
 1.47025;1.00569;-0.30001;,
 1.76947;0.35315;-0.00637;,
 1.80425;0.00000;-0.00637;,
 1.76947;0.00000;-0.35952;,
 1.73535;0.35315;-0.35274;,
 1.76947;-0.35315;-0.00637;,
 1.66646;-0.69273;-0.00637;,
 1.63432;-0.69273;-0.33264;,
 1.73535;-0.35315;-0.35274;,
 1.49917;-1.00569;-0.00637;,
 1.27405;-1.28000;-0.00637;,
 1.24946;-1.28000;-0.25609;,
 1.47025;-1.00569;-0.30001;,
 0.99974;-1.50512;-0.00637;,
 0.98042;-1.50512;-0.20257;,
 0.68678;1.67241;-0.00637;,
 0.67347;1.67241;-0.14152;,
 0.34720;1.77542;-0.00637;,
 0.34042;1.77542;-0.07527;,
 0.32032;-1.77542;-0.14152;,
 0.63405;-1.67240;-0.27147;,
 1.17662;1.28000;-0.49621;,
 0.92319;1.50512;-0.39123;,
 1.53915;0.69273;-0.64637;,
 1.38460;1.00569;-0.58236;,
 1.66646;0.00000;-0.69910;,
 1.63432;0.35315;-0.68579;,
 1.53915;-0.69273;-0.64637;,
 1.63432;-0.35315;-0.68579;,
 1.17662;-1.28000;-0.49621;,
 1.38460;-1.00569;-0.58236;,
 0.92319;-1.50512;-0.39123;,
 0.63405;1.67241;-0.27147;,
 0.32032;1.77542;-0.14152;,
 0.28769;-1.77542;-0.20257;,
 0.57004;-1.67240;-0.39123;,
 1.05833;1.28000;-0.71750;,
 0.83025;1.50512;-0.56510;,
 1.38460;0.69273;-0.93551;,
 1.24552;1.00569;-0.84257;,
 1.49917;0.00000;-1.01206;,
 1.47025;0.35315;-0.99274;,
 1.38460;-0.69273;-0.93551;,
 1.47025;-0.35315;-0.99274;,
 1.05833;-1.28000;-0.71750;,
 1.24552;-1.00569;-0.84257;,
 0.83025;-1.50512;-0.56510;,
 0.57004;1.67241;-0.39123;,
 0.28769;1.77542;-0.20257;,
 0.24377;-1.77542;-0.25609;,
 0.48389;-1.67240;-0.49621;,
 0.89915;1.28000;-0.91147;,
 0.70518;1.50512;-0.71750;,
 1.17662;0.69273;-1.18894;,
 1.05833;1.00569;-1.07065;,
 1.27405;0.00000;-1.28637;,
 1.24946;0.35315;-1.26178;,
 1.17662;-0.69273;-1.18894;,
 1.24946;-0.35315;-1.26178;,
 0.89915;-1.28000;-0.91147;,
 1.05833;-1.00569;-1.07065;,
 0.70518;-1.50512;-0.71750;,
 0.48389;1.67241;-0.49621;,
 0.24377;1.77542;-0.25609;,
 0.19025;-1.77542;-0.30001;,
 0.37891;-1.67240;-0.58236;,
 0.70518;1.28000;-1.07065;,
 0.55278;1.50512;-0.84257;,
 0.92319;0.69273;-1.39692;,
 0.83025;1.00569;-1.25784;,
 0.99974;0.00000;-1.51150;,
 0.98042;0.35315;-1.48257;,
 0.92319;-0.69273;-1.39692;,
 0.98042;-0.35315;-1.48257;,
 0.70518;-1.28000;-1.07065;,
 0.83025;-1.00569;-1.25784;,
 0.55278;-1.50512;-0.84257;,
 0.37891;1.67241;-0.58236;,
 0.19025;1.77542;-0.30001;,
 0.12920;-1.77542;-0.33264;,
 0.25915;-1.67240;-0.64637;,
 0.48389;1.28000;-1.18894;,
 0.37891;1.50512;-0.93551;,
 0.63405;0.69273;-1.55147;,
 0.57004;1.00569;-1.39692;,
 0.68678;0.00000;-1.67878;,
 0.67347;0.35315;-1.64664;,
 0.63405;-0.69273;-1.55147;,
 0.67347;-0.35315;-1.64664;,
 0.48389;-1.28000;-1.18894;,
 0.57004;-1.00569;-1.39692;,
 0.37891;-1.50512;-0.93551;,
 0.25915;1.67241;-0.64637;,
 0.12920;1.77542;-0.33264;,
 0.06295;-1.77542;-0.35274;,
 0.12920;-1.67240;-0.68579;,
 0.24377;1.28000;-1.26178;,
 0.19025;1.50512;-0.99274;,
 0.32032;0.69273;-1.64664;,
 0.28769;1.00569;-1.48257;,
 0.34720;0.00000;-1.78179;,
 0.34042;0.35315;-1.74767;,
 0.32032;-0.69273;-1.64664;,
 0.34042;-0.35315;-1.74767;,
 0.24377;-1.28000;-1.26178;,
 0.28769;-1.00569;-1.48257;,
 0.19025;-1.50512;-0.99274;,
 0.12920;1.67241;-0.68579;,
 0.06295;1.77542;-0.35274;,
 -0.00595;-1.77542;-0.35952;,
 -0.00595;-1.67240;-0.69910;,
 -0.00595;1.28000;-1.28637;,
 -0.00595;1.50512;-1.01206;,
 -0.00595;0.69273;-1.67878;,
 -0.00595;1.00569;-1.51149;,
 -0.00595;0.00000;-1.81657;,
 -0.00595;0.35315;-1.78179;,
 -0.00595;-0.69273;-1.67878;,
 -0.00595;-0.35315;-1.78179;,
 -0.00595;-1.28000;-1.28637;,
 -0.00595;-1.00569;-1.51149;,
 -0.00595;-1.50512;-1.01206;,
 -0.00595;1.67241;-0.69910;,
 -0.00595;1.77542;-0.35952;,
 -0.00595;-1.67240;-0.69910;,
 -0.00595;-1.77542;-0.35952;,
 -0.07485;-1.77542;-0.35274;,
 -0.14110;-1.67240;-0.68579;,
 -0.25567;1.28000;-1.26178;,
 -0.20215;1.50512;-0.99274;,
 -0.33222;0.69273;-1.64664;,
 -0.29959;1.00569;-1.48257;,
 -0.35910;0.00000;-1.78179;,
 -0.35232;0.35315;-1.74767;,
 -0.00595;-0.35315;-1.78179;,
 -0.00595;-0.69273;-1.67878;,
 -0.33222;-0.69273;-1.64664;,
 -0.35232;-0.35315;-1.74767;,
 -0.00595;-1.00569;-1.51149;,
 -0.00595;-1.28000;-1.28637;,
 -0.25567;-1.28000;-1.26178;,
 -0.29959;-1.00569;-1.48257;,
 -0.00595;-1.50512;-1.01206;,
 -0.20215;-1.50512;-0.99274;,
 -0.14110;1.67241;-0.68579;,
 -0.07485;1.77542;-0.35274;,
 -0.14110;-1.77542;-0.33264;,
 -0.27105;-1.67240;-0.64637;,
 -0.49579;1.28000;-1.18894;,
 -0.39081;1.50512;-0.93551;,
 -0.64595;0.69273;-1.55147;,
 -0.58194;1.00569;-1.39692;,
 -0.69868;0.00000;-1.67878;,
 -0.68537;0.35315;-1.64664;,
 -0.64595;-0.69273;-1.55147;,
 -0.68537;-0.35315;-1.64664;,
 -0.49579;-1.28000;-1.18894;,
 -0.58194;-1.00569;-1.39692;,
 -0.39081;-1.50512;-0.93551;,
 -0.27105;1.67241;-0.64637;,
 -0.14110;1.77542;-0.33264;,
 -0.20215;-1.77542;-0.30001;,
 -0.39081;-1.67240;-0.58236;,
 -0.71708;1.28000;-1.07065;,
 -0.56468;1.50512;-0.84257;,
 -0.93509;0.69273;-1.39692;,
 -0.84215;1.00569;-1.25784;,
 -1.01164;0.00000;-1.51149;,
 -0.99232;0.35315;-1.48257;,
 -0.93509;-0.69273;-1.39692;,
 -0.99232;-0.35315;-1.48257;,
 -0.71708;-1.28000;-1.07065;,
 -0.84215;-1.00569;-1.25784;,
 -0.56468;-1.50512;-0.84257;,
 -0.39081;1.67241;-0.58236;,
 -0.20215;1.77542;-0.30001;,
 -0.25567;-1.77542;-0.25609;,
 -0.49579;-1.67240;-0.49621;,
 -0.91105;1.28000;-0.91147;,
 -0.71708;1.50512;-0.71750;,
 -1.18852;0.69273;-1.18894;,
 -1.07023;1.00569;-1.07065;,
 -1.28595;0.00000;-1.28637;,
 -1.26136;0.35315;-1.26178;,
 -1.18852;-0.69273;-1.18894;,
 -1.26136;-0.35315;-1.26178;,
 -0.91105;-1.28000;-0.91147;,
 -1.07023;-1.00569;-1.07065;,
 -0.71708;-1.50512;-0.71750;,
 -0.49579;1.67241;-0.49621;,
 -0.25567;1.77542;-0.25609;,
 -0.29959;-1.77542;-0.20257;,
 -0.58194;-1.67240;-0.39123;,
 -1.07023;1.28000;-0.71750;,
 -0.84215;1.50512;-0.56510;,
 -1.39650;0.69273;-0.93551;,
 -1.25741;1.00569;-0.84257;,
 -1.51107;0.00000;-1.01206;,
 -1.48215;0.35315;-0.99274;,
 -1.39650;-0.69273;-0.93551;,
 -1.48215;-0.35315;-0.99274;,
 -1.07023;-1.28000;-0.71750;,
 -1.25742;-1.00569;-0.84257;,
 -0.84215;-1.50512;-0.56510;,
 -0.58194;1.67241;-0.39123;,
 -0.29959;1.77542;-0.20257;,
 -0.33222;-1.77542;-0.14152;,
 -0.64595;-1.67240;-0.27147;,
 -1.18852;1.28000;-0.49621;,
 -0.93509;1.50512;-0.39123;,
 -1.55105;0.69273;-0.64637;,
 -1.39650;1.00569;-0.58236;,
 -1.67835;0.00000;-0.69910;,
 -1.64622;0.35315;-0.68579;,
 -1.55105;-0.69273;-0.64637;,
 -1.64622;-0.35315;-0.68579;,
 -1.18852;-1.28000;-0.49621;,
 -1.39650;-1.00569;-0.58236;,
 -0.93509;-1.50512;-0.39123;,
 -0.64595;1.67241;-0.27147;,
 -0.33222;1.77542;-0.14152;,
 -0.35232;-1.77542;-0.07527;,
 -0.68537;-1.67240;-0.14152;,
 -1.26136;1.28000;-0.25609;,
 -0.99232;1.50512;-0.20257;,
 -1.64622;0.69273;-0.33264;,
 -1.48215;1.00569;-0.30001;,
 -1.78137;0.00000;-0.35952;,
 -1.74725;0.35315;-0.35274;,
 -1.64622;-0.69273;-0.33264;,
 -1.74725;-0.35315;-0.35274;,
 -1.26136;-1.28000;-0.25609;,
 -1.48215;-1.00569;-0.30001;,
 -0.99232;-1.50512;-0.20257;,
 -0.68537;1.67241;-0.14152;,
 -0.35232;1.77542;-0.07527;,
 -0.35910;-1.77542;-0.00637;,
 -0.69868;-1.67240;-0.00637;,
 -1.28595;1.28000;-0.00637;,
 -1.01164;1.50512;-0.00637;,
 -1.67835;0.69273;-0.00637;,
 -1.51107;1.00569;-0.00637;,
 -1.81615;0.00000;-0.00637;,
 -1.78136;0.35315;-0.00637;,
 -1.67835;-0.69273;-0.00637;,
 -1.78136;-0.35315;-0.00637;,
 -1.28595;-1.28000;-0.00637;,
 -1.51107;-1.00569;-0.00637;,
 -1.01164;-1.50512;-0.00637;,
 -0.69868;1.67241;-0.00637;,
 -0.35910;1.77542;-0.00637;,
 -0.00595;1.81020;-0.00637;,
 -0.00595;-1.81020;-0.00637;,
 -0.00595;-1.81020;-0.00637;,
 1.92382;1.95209;-0.00665;,
 1.92382;-1.58256;-0.00665;,
 -0.03116;-1.94579;-0.00665;,
 -0.03116;2.86668;-0.00665;,
 -1.98614;1.95209;-0.00665;,
 -1.98614;-1.58256;-0.00665;,
 -1.80911;-1.29191;6.78615;,
 -1.80911;1.77461;6.78615;,
 -0.03116;2.56807;6.78615;,
 -0.03116;-1.60703;6.78615;,
 1.74679;-1.29191;6.78615;,
 1.74679;1.77461;6.78615;,
 1.74679;1.77461;6.78615;,
 1.74679;-1.29191;6.78615;,
 1.92382;-1.58256;-0.00665;,
 1.92382;1.95209;-0.00665;,
 -0.03116;-1.60703;6.78615;,
 -0.03116;-1.94579;-0.00665;,
 -0.03116;2.86668;-0.00665;,
 -0.03116;2.56807;6.78615;,
 -1.98614;-1.58256;-0.00665;,
 -1.98614;1.95209;-0.00665;,
 -0.03116;-1.94579;-0.00665;,
 -0.03116;-1.60703;6.78615;,
 -1.80911;1.77461;6.78615;,
 -1.80911;-1.29191;6.78615;;
 
 266;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,0,3,25;,
 4;26,4,7,27;,
 4;5,8,11,6;,
 4;9,12,15,10;,
 4;13,16,19,14;,
 4;17,20,23,18;,
 4;21,24,25,22;,
 4;28,26,27,29;,
 4;3,2,30,31;,
 4;7,6,32,33;,
 4;11,10,34,35;,
 4;15,14,36,37;,
 4;19,18,38,39;,
 4;23,22,40,41;,
 4;25,3,31,42;,
 4;27,7,33,43;,
 4;6,11,35,32;,
 4;10,15,37,34;,
 4;14,19,39,36;,
 4;18,23,41,38;,
 4;22,25,42,40;,
 4;29,27,43,44;,
 4;31,30,45,46;,
 4;33,32,47,48;,
 4;35,34,49,50;,
 4;37,36,51,52;,
 4;39,38,53,54;,
 4;41,40,55,56;,
 4;42,31,46,57;,
 4;43,33,48,58;,
 4;32,35,50,47;,
 4;34,37,52,49;,
 4;36,39,54,51;,
 4;38,41,56,53;,
 4;40,42,57,55;,
 4;44,43,58,59;,
 4;46,45,60,61;,
 4;48,47,62,63;,
 4;50,49,64,65;,
 4;52,51,66,67;,
 4;54,53,68,69;,
 4;56,55,70,71;,
 4;57,46,61,72;,
 4;58,48,63,73;,
 4;47,50,65,62;,
 4;49,52,67,64;,
 4;51,54,69,66;,
 4;53,56,71,68;,
 4;55,57,72,70;,
 4;59,58,73,74;,
 4;61,60,75,76;,
 4;63,62,77,78;,
 4;65,64,79,80;,
 4;67,66,81,82;,
 4;69,68,83,84;,
 4;71,70,85,86;,
 4;72,61,76,87;,
 4;73,63,78,88;,
 4;62,65,80,77;,
 4;64,67,82,79;,
 4;66,69,84,81;,
 4;68,71,86,83;,
 4;70,72,87,85;,
 4;74,73,88,89;,
 4;76,75,90,91;,
 4;78,77,92,93;,
 4;80,79,94,95;,
 4;82,81,96,97;,
 4;84,83,98,99;,
 4;86,85,100,101;,
 4;87,76,91,102;,
 4;88,78,93,103;,
 4;77,80,95,92;,
 4;79,82,97,94;,
 4;81,84,99,96;,
 4;83,86,101,98;,
 4;85,87,102,100;,
 4;89,88,103,104;,
 4;91,90,105,106;,
 4;93,92,107,108;,
 4;95,94,109,110;,
 4;97,96,111,112;,
 4;99,98,113,114;,
 4;101,100,115,116;,
 4;102,91,106,117;,
 4;103,93,108,118;,
 4;92,95,110,107;,
 4;94,97,112,109;,
 4;96,99,114,111;,
 4;98,101,116,113;,
 4;100,102,117,115;,
 4;104,103,118,119;,
 4;106,105,120,121;,
 4;108,107,122,123;,
 4;110,109,124,125;,
 4;112,111,126,127;,
 4;114,113,128,129;,
 4;116,115,130,131;,
 4;117,106,121,132;,
 4;118,108,123,133;,
 4;107,110,125,122;,
 4;109,112,127,124;,
 4;111,114,129,126;,
 4;113,116,131,128;,
 4;115,117,132,130;,
 4;119,118,133,134;,
 4;135,136,137,138;,
 4;123,122,139,140;,
 4;125,124,141,142;,
 4;127,126,143,144;,
 4;145,146,147,148;,
 4;149,150,151,152;,
 4;153,135,138,154;,
 4;133,123,140,155;,
 4;122,125,142,139;,
 4;124,127,144,141;,
 4;126,145,148,143;,
 4;146,149,152,147;,
 4;150,153,154,151;,
 4;134,133,155,156;,
 4;138,137,157,158;,
 4;140,139,159,160;,
 4;142,141,161,162;,
 4;144,143,163,164;,
 4;148,147,165,166;,
 4;152,151,167,168;,
 4;154,138,158,169;,
 4;155,140,160,170;,
 4;139,142,162,159;,
 4;141,144,164,161;,
 4;143,148,166,163;,
 4;147,152,168,165;,
 4;151,154,169,167;,
 4;156,155,170,171;,
 4;158,157,172,173;,
 4;160,159,174,175;,
 4;162,161,176,177;,
 4;164,163,178,179;,
 4;166,165,180,181;,
 4;168,167,182,183;,
 4;169,158,173,184;,
 4;170,160,175,185;,
 4;159,162,177,174;,
 4;161,164,179,176;,
 4;163,166,181,178;,
 4;165,168,183,180;,
 4;167,169,184,182;,
 4;171,170,185,186;,
 4;173,172,187,188;,
 4;175,174,189,190;,
 4;177,176,191,192;,
 4;179,178,193,194;,
 4;181,180,195,196;,
 4;183,182,197,198;,
 4;184,173,188,199;,
 4;185,175,190,200;,
 4;174,177,192,189;,
 4;176,179,194,191;,
 4;178,181,196,193;,
 4;180,183,198,195;,
 4;182,184,199,197;,
 4;186,185,200,201;,
 4;188,187,202,203;,
 4;190,189,204,205;,
 4;192,191,206,207;,
 4;194,193,208,209;,
 4;196,195,210,211;,
 4;198,197,212,213;,
 4;199,188,203,214;,
 4;200,190,205,215;,
 4;189,192,207,204;,
 4;191,194,209,206;,
 4;193,196,211,208;,
 4;195,198,213,210;,
 4;197,199,214,212;,
 4;201,200,215,216;,
 4;203,202,217,218;,
 4;205,204,219,220;,
 4;207,206,221,222;,
 4;209,208,223,224;,
 4;211,210,225,226;,
 4;213,212,227,228;,
 4;214,203,218,229;,
 4;215,205,220,230;,
 4;204,207,222,219;,
 4;206,209,224,221;,
 4;208,211,226,223;,
 4;210,213,228,225;,
 4;212,214,229,227;,
 4;216,215,230,231;,
 4;218,217,232,233;,
 4;220,219,234,235;,
 4;222,221,236,237;,
 4;224,223,238,239;,
 4;226,225,240,241;,
 4;228,227,242,243;,
 4;229,218,233,244;,
 4;230,220,235,245;,
 4;219,222,237,234;,
 4;221,224,239,236;,
 4;223,226,241,238;,
 4;225,228,243,240;,
 4;227,229,244,242;,
 4;231,230,245,246;,
 4;233,232,247,248;,
 4;235,234,249,250;,
 4;237,236,251,252;,
 4;239,238,253,254;,
 4;241,240,255,256;,
 4;243,242,257,258;,
 4;244,233,248,259;,
 4;245,235,250,260;,
 4;234,237,252,249;,
 4;236,239,254,251;,
 4;238,241,256,253;,
 4;240,243,258,255;,
 4;242,244,259,257;,
 4;246,245,260,261;,
 3;262,28,29;,
 3;1,263,2;,
 3;262,29,44;,
 3;2,263,30;,
 3;262,44,59;,
 3;30,263,45;,
 3;262,59,74;,
 3;45,263,60;,
 3;262,74,89;,
 3;60,263,75;,
 3;262,89,104;,
 3;75,263,90;,
 3;262,104,119;,
 3;90,263,105;,
 3;262,119,134;,
 3;105,263,120;,
 3;262,134,156;,
 3;136,264,137;,
 3;262,156,171;,
 3;137,264,157;,
 3;262,171,186;,
 3;157,264,172;,
 3;262,186,201;,
 3;172,264,187;,
 3;262,201,216;,
 3;187,264,202;,
 3;262,216,231;,
 3;202,264,217;,
 3;262,231,246;,
 3;217,264,232;,
 3;262,246,261;,
 3;232,264,247;,
 4;265,266,267,268;,
 4;269,270,271,272;,
 4;273,274,275,276;,
 4;277,278,279,280;,
 4;279,278,281,282;,
 4;277,280,283,284;,
 4;268,267,285,286;,
 4;287,288,271,270;,
 4;284,283,269,272;,
 4;289,290,274,273;;
 
 MeshMaterialList {
  1;
  266;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.352000;0.352000;0.352000;1.000000;;
   15.000000;
   0.240000;0.240000;0.240000;;
   0.672000;0.672000;0.672000;;
   TextureFilename {
    "data\\TEXTURE\\UV\\player\\L\\L_finger2_0_UV.jpg";
   }
  }
 }
 MeshNormals {
  267;
  0.195047;-0.980606;-0.019199;,
  0.382393;-0.923232;-0.037652;,
  0.554703;-0.830253;-0.054624;,
  0.705368;-0.705429;-0.069461;,
  0.828669;-0.553756;-0.081615;,
  0.920062;-0.381145;-0.090629;,
  0.976236;-0.194205;-0.096161;,
  0.995184;-0.000000;-0.098022;,
  0.976236;0.194205;-0.096161;,
  0.920062;0.381146;-0.090629;,
  0.828669;0.553757;-0.081615;,
  0.705368;0.705430;-0.069461;,
  0.554713;0.830246;-0.054625;,
  0.382392;0.923233;-0.037652;,
  0.195034;0.980608;-0.019198;,
  0.191334;-0.980787;-0.038056;,
  0.375311;-0.923888;-0.074651;,
  0.544858;-0.831496;-0.108374;,
  0.693490;-0.707138;-0.137939;,
  0.815469;-0.555607;-0.162207;,
  0.906112;-0.382718;-0.180243;,
  0.961935;-0.195112;-0.191346;,
  0.980785;-0.000000;-0.195092;,
  0.961935;0.195112;-0.191346;,
  0.906112;0.382718;-0.180243;,
  0.815469;0.555607;-0.162207;,
  0.693490;0.707139;-0.137938;,
  0.544868;0.831489;-0.108376;,
  0.375310;0.923888;-0.074651;,
  0.191321;0.980790;-0.038053;,
  0.180233;-0.980787;-0.074655;,
  0.353536;-0.923887;-0.146440;,
  0.513243;-0.831495;-0.212597;,
  0.653248;-0.707141;-0.270590;,
  0.768157;-0.555604;-0.318181;,
  0.853542;-0.382713;-0.353549;,
  0.906122;-0.195112;-0.375333;,
  0.923877;0.000000;-0.382690;,
  0.906122;0.195112;-0.375333;,
  0.853542;0.382713;-0.353549;,
  0.768157;0.555604;-0.318181;,
  0.653248;0.707141;-0.270590;,
  0.513252;0.831488;-0.212601;,
  0.353536;0.923887;-0.146440;,
  0.180220;0.980789;-0.074650;,
  0.162207;-0.980787;-0.108377;,
  0.318179;-0.923886;-0.212594;,
  0.461910;-0.831493;-0.308639;,
  0.587906;-0.707142;-0.392831;,
  0.691321;-0.555606;-0.461928;,
  0.768168;-0.382713;-0.513272;,
  0.815491;-0.195106;-0.544894;,
  0.831467;0.000000;-0.555574;,
  0.815491;0.195106;-0.544894;,
  0.768168;0.382713;-0.513272;,
  0.691321;0.555606;-0.461928;,
  0.587907;0.707142;-0.392831;,
  0.461919;0.831486;-0.308645;,
  0.318178;0.923887;-0.212594;,
  0.162196;0.980790;-0.108370;,
  0.137943;-0.980787;-0.137943;,
  0.270587;-0.923886;-0.270587;,
  0.392823;-0.831493;-0.392823;,
  0.499975;-0.707142;-0.499975;,
  0.587920;-0.555608;-0.587920;,
  0.653272;-0.382716;-0.653272;,
  0.693520;-0.195105;-0.693516;,
  0.707111;-0.000000;-0.707103;,
  0.693520;0.195105;-0.693516;,
  0.653272;0.382716;-0.653272;,
  0.587920;0.555607;-0.587921;,
  0.499975;0.707142;-0.499975;,
  0.392829;0.831487;-0.392830;,
  0.270586;0.923886;-0.270586;,
  0.137934;0.980790;-0.137934;,
  0.108377;-0.980787;-0.162207;,
  0.212594;-0.923886;-0.318180;,
  0.308639;-0.831493;-0.461911;,
  0.392830;-0.707142;-0.587907;,
  0.461927;-0.555607;-0.691321;,
  0.513271;-0.382714;-0.768169;,
  0.544892;-0.195112;-0.815490;,
  0.555572;-0.000000;-0.831468;,
  0.544892;0.195112;-0.815490;,
  0.513271;0.382713;-0.768169;,
  0.461928;0.555606;-0.691321;,
  0.392830;0.707142;-0.587907;,
  0.308644;0.831486;-0.461919;,
  0.212594;0.923886;-0.318179;,
  0.108370;0.980790;-0.162196;,
  0.074656;-0.980787;-0.180233;,
  0.146441;-0.923887;-0.353537;,
  0.212598;-0.831495;-0.513243;,
  0.270590;-0.707141;-0.653248;,
  0.318182;-0.555604;-0.768157;,
  0.353549;-0.382713;-0.853542;,
  0.375329;-0.195115;-0.906122;,
  0.382683;-0.000000;-0.923880;,
  0.375329;0.195115;-0.906122;,
  0.353549;0.382713;-0.853542;,
  0.318182;0.555604;-0.768157;,
  0.270590;0.707141;-0.653248;,
  0.212601;0.831488;-0.513253;,
  0.146440;0.923887;-0.353536;,
  0.074650;0.980789;-0.180220;,
  0.038056;-0.980787;-0.191334;,
  0.074651;-0.923888;-0.375312;,
  0.108375;-0.831496;-0.544858;,
  0.137939;-0.707139;-0.693490;,
  0.162207;-0.555607;-0.815469;,
  0.180244;-0.382717;-0.906112;,
  0.191346;-0.195112;-0.961935;,
  0.195093;-0.000000;-0.980785;,
  0.191346;0.195112;-0.961935;,
  0.180244;0.382717;-0.906112;,
  0.162207;0.555606;-0.815469;,
  0.137939;0.707138;-0.693490;,
  0.108377;0.831489;-0.544868;,
  0.074651;0.923888;-0.375311;,
  0.038053;0.980790;-0.191321;,
  -0.000000;-0.980786;-0.195084;,
  -0.000000;-0.923887;-0.382665;,
  -0.000000;-0.831494;-0.555533;,
  0.000000;-0.707138;-0.707076;,
  0.000000;-0.555610;-0.831443;,
  0.000000;-0.382720;-0.923865;,
  0.000000;-0.195109;-0.980781;,
  0.000000;-0.000000;-1.000000;,
  0.000000;0.195109;-0.980781;,
  0.000000;0.382720;-0.923865;,
  0.000000;0.555609;-0.831443;,
  0.000000;0.707138;-0.707076;,
  -0.000000;0.831487;-0.555544;,
  0.000000;0.923887;-0.382664;,
  0.000000;0.980789;-0.195071;,
  -0.038056;-0.980787;-0.191334;,
  -0.074651;-0.923888;-0.375311;,
  -0.108375;-0.831496;-0.544858;,
  -0.137939;-0.707139;-0.693490;,
  -0.162207;-0.555607;-0.815469;,
  -0.180244;-0.382717;-0.906112;,
  -0.191346;-0.195112;-0.961935;,
  -0.195093;-0.000000;-0.980785;,
  -0.191346;0.195112;-0.961935;,
  -0.180244;0.382717;-0.906112;,
  -0.162207;0.555607;-0.815469;,
  -0.137939;0.707139;-0.693490;,
  -0.108377;0.831489;-0.544868;,
  -0.074651;0.923888;-0.375310;,
  -0.038053;0.980790;-0.191321;,
  -0.074655;-0.980787;-0.180233;,
  -0.146440;-0.923887;-0.353537;,
  -0.212598;-0.831495;-0.513243;,
  -0.270590;-0.707141;-0.653248;,
  -0.318182;-0.555604;-0.768157;,
  -0.353549;-0.382713;-0.853542;,
  -0.375333;-0.195112;-0.906122;,
  -0.382689;-0.000000;-0.923877;,
  -0.375333;0.195112;-0.906122;,
  -0.353549;0.382713;-0.853542;,
  -0.318182;0.555604;-0.768156;,
  -0.270590;0.707141;-0.653248;,
  -0.212601;0.831488;-0.513253;,
  -0.146440;0.923887;-0.353536;,
  -0.074650;0.980789;-0.180220;,
  -0.108377;-0.980787;-0.162207;,
  -0.212594;-0.923886;-0.318180;,
  -0.308639;-0.831493;-0.461911;,
  -0.392831;-0.707142;-0.587907;,
  -0.461928;-0.555606;-0.691321;,
  -0.513272;-0.382713;-0.768168;,
  -0.544894;-0.195106;-0.815491;,
  -0.555574;-0.000000;-0.831467;,
  -0.544894;0.195106;-0.815491;,
  -0.513272;0.382714;-0.768168;,
  -0.461928;0.555606;-0.691321;,
  -0.392831;0.707142;-0.587907;,
  -0.308645;0.831486;-0.461920;,
  -0.212594;0.923886;-0.318179;,
  -0.108370;0.980790;-0.162196;,
  -0.137943;-0.980787;-0.137943;,
  -0.270587;-0.923886;-0.270587;,
  -0.392823;-0.831493;-0.392823;,
  -0.499975;-0.707142;-0.499975;,
  -0.587920;-0.555607;-0.587920;,
  -0.653272;-0.382716;-0.653272;,
  -0.693518;-0.195102;-0.693518;,
  -0.707107;-0.000000;-0.707107;,
  -0.693518;0.195102;-0.693518;,
  -0.653273;0.382719;-0.653269;,
  -0.587924;0.555608;-0.587916;,
  -0.499978;0.707141;-0.499974;,
  -0.392830;0.831486;-0.392830;,
  -0.270587;0.923886;-0.270587;,
  -0.137934;0.980790;-0.137934;,
  -0.162207;-0.980787;-0.108377;,
  -0.318180;-0.923886;-0.212594;,
  -0.461911;-0.831493;-0.308639;,
  -0.587906;-0.707142;-0.392830;,
  -0.691320;-0.555607;-0.461928;,
  -0.768168;-0.382714;-0.513272;,
  -0.815493;-0.195103;-0.544891;,
  -0.831471;-0.000000;-0.555568;,
  -0.815493;0.195103;-0.544891;,
  -0.768167;0.382718;-0.513271;,
  -0.691320;0.555606;-0.461929;,
  -0.587909;0.707139;-0.392833;,
  -0.461919;0.831486;-0.308645;,
  -0.318179;0.923886;-0.212594;,
  -0.162196;0.980790;-0.108370;,
  -0.000000;0.998015;-0.062975;,
  -0.180233;-0.980787;-0.074655;,
  -0.353537;-0.923887;-0.146440;,
  -0.513244;-0.831495;-0.212597;,
  -0.653248;-0.707141;-0.270590;,
  -0.768156;-0.555605;-0.318181;,
  -0.853542;-0.382713;-0.353549;,
  -0.906123;-0.195106;-0.375334;,
  -0.923877;-0.000000;-0.382690;,
  -0.906123;0.195106;-0.375334;,
  -0.853540;0.382716;-0.353552;,
  -0.768154;0.555605;-0.318187;,
  -0.653248;0.707139;-0.270593;,
  -0.513253;0.831488;-0.212601;,
  -0.353536;0.923887;-0.146440;,
  -0.180220;0.980789;-0.074650;,
  -0.191334;-0.980787;-0.038056;,
  -0.375311;-0.923888;-0.074651;,
  -0.544858;-0.831495;-0.108374;,
  -0.693490;-0.707138;-0.137938;,
  -0.815471;-0.555605;-0.162204;,
  -0.906115;-0.382715;-0.180233;,
  -0.961936;-0.195112;-0.191339;,
  -0.980785;-0.000000;-0.195092;,
  -0.961936;0.195112;-0.191339;,
  -0.906115;0.382715;-0.180233;,
  -0.815471;0.555605;-0.162204;,
  -0.693490;0.707139;-0.137938;,
  -0.544868;0.831489;-0.108376;,
  -0.375311;0.923888;-0.074651;,
  -0.191321;0.980790;-0.038053;,
  -0.195047;-0.980606;-0.019199;,
  -0.382393;-0.923232;-0.037652;,
  -0.554704;-0.830253;-0.054624;,
  -0.705368;-0.705429;-0.069461;,
  -0.828673;-0.553751;-0.081609;,
  -0.920066;-0.381140;-0.090609;,
  -0.976237;-0.194212;-0.096140;,
  -0.995186;0.000000;-0.098008;,
  -0.976237;0.194212;-0.096140;,
  -0.920066;0.381140;-0.090609;,
  -0.828673;0.553751;-0.081609;,
  -0.705368;0.705430;-0.069461;,
  -0.554714;0.830246;-0.054625;,
  -0.382393;0.923232;-0.037652;,
  -0.195034;0.980609;-0.019198;,
  -0.000000;-0.998015;-0.062975;,
  0.000000;0.000000;-1.000000;,
  -0.999661;0.000000;0.026053;,
  0.000000;-0.000000;1.000000;,
  0.999661;0.000000;0.026053;,
  0.178391;-0.982792;0.047912;,
  -0.000000;-0.998814;0.048693;,
  0.415371;0.908894;0.037125;,
  -0.000000;0.999167;0.040812;,
  -0.178391;-0.982792;0.047912;,
  -0.415371;0.908894;0.037125;;
  266;
  4;1,0,15,16;,
  4;12,11,26,27;,
  4;10,9,24,25;,
  4;8,7,22,23;,
  4;6,5,20,21;,
  4;4,3,18,19;,
  4;2,1,16,17;,
  4;13,12,27,28;,
  4;11,10,25,26;,
  4;9,8,23,24;,
  4;7,6,21,22;,
  4;5,4,19,20;,
  4;3,2,17,18;,
  4;14,13,28,29;,
  4;16,15,30,31;,
  4;27,26,41,42;,
  4;25,24,39,40;,
  4;23,22,37,38;,
  4;21,20,35,36;,
  4;19,18,33,34;,
  4;17,16,31,32;,
  4;28,27,42,43;,
  4;26,25,40,41;,
  4;24,23,38,39;,
  4;22,21,36,37;,
  4;20,19,34,35;,
  4;18,17,32,33;,
  4;29,28,43,44;,
  4;31,30,45,46;,
  4;42,41,56,57;,
  4;40,39,54,55;,
  4;38,37,52,53;,
  4;36,35,50,51;,
  4;34,33,48,49;,
  4;32,31,46,47;,
  4;43,42,57,58;,
  4;41,40,55,56;,
  4;39,38,53,54;,
  4;37,36,51,52;,
  4;35,34,49,50;,
  4;33,32,47,48;,
  4;44,43,58,59;,
  4;46,45,60,61;,
  4;57,56,71,72;,
  4;55,54,69,70;,
  4;53,52,67,68;,
  4;51,50,65,66;,
  4;49,48,63,64;,
  4;47,46,61,62;,
  4;58,57,72,73;,
  4;56,55,70,71;,
  4;54,53,68,69;,
  4;52,51,66,67;,
  4;50,49,64,65;,
  4;48,47,62,63;,
  4;59,58,73,74;,
  4;61,60,75,76;,
  4;72,71,86,87;,
  4;70,69,84,85;,
  4;68,67,82,83;,
  4;66,65,80,81;,
  4;64,63,78,79;,
  4;62,61,76,77;,
  4;73,72,87,88;,
  4;71,70,85,86;,
  4;69,68,83,84;,
  4;67,66,81,82;,
  4;65,64,79,80;,
  4;63,62,77,78;,
  4;74,73,88,89;,
  4;76,75,90,91;,
  4;87,86,101,102;,
  4;85,84,99,100;,
  4;83,82,97,98;,
  4;81,80,95,96;,
  4;79,78,93,94;,
  4;77,76,91,92;,
  4;88,87,102,103;,
  4;86,85,100,101;,
  4;84,83,98,99;,
  4;82,81,96,97;,
  4;80,79,94,95;,
  4;78,77,92,93;,
  4;89,88,103,104;,
  4;91,90,105,106;,
  4;102,101,116,117;,
  4;100,99,114,115;,
  4;98,97,112,113;,
  4;96,95,110,111;,
  4;94,93,108,109;,
  4;92,91,106,107;,
  4;103,102,117,118;,
  4;101,100,115,116;,
  4;99,98,113,114;,
  4;97,96,111,112;,
  4;95,94,109,110;,
  4;93,92,107,108;,
  4;104,103,118,119;,
  4;106,105,120,121;,
  4;117,116,131,132;,
  4;115,114,129,130;,
  4;113,112,127,128;,
  4;111,110,125,126;,
  4;109,108,123,124;,
  4;107,106,121,122;,
  4;118,117,132,133;,
  4;116,115,130,131;,
  4;114,113,128,129;,
  4;112,111,126,127;,
  4;110,109,124,125;,
  4;108,107,122,123;,
  4;119,118,133,134;,
  4;121,120,135,136;,
  4;132,131,146,147;,
  4;130,129,144,145;,
  4;128,127,142,143;,
  4;126,125,140,141;,
  4;124,123,138,139;,
  4;122,121,136,137;,
  4;133,132,147,148;,
  4;131,130,145,146;,
  4;129,128,143,144;,
  4;127,126,141,142;,
  4;125,124,139,140;,
  4;123,122,137,138;,
  4;134,133,148,149;,
  4;136,135,150,151;,
  4;147,146,161,162;,
  4;145,144,159,160;,
  4;143,142,157,158;,
  4;141,140,155,156;,
  4;139,138,153,154;,
  4;137,136,151,152;,
  4;148,147,162,163;,
  4;146,145,160,161;,
  4;144,143,158,159;,
  4;142,141,156,157;,
  4;140,139,154,155;,
  4;138,137,152,153;,
  4;149,148,163,164;,
  4;151,150,165,166;,
  4;162,161,176,177;,
  4;160,159,174,175;,
  4;158,157,172,173;,
  4;156,155,170,171;,
  4;154,153,168,169;,
  4;152,151,166,167;,
  4;163,162,177,178;,
  4;161,160,175,176;,
  4;159,158,173,174;,
  4;157,156,171,172;,
  4;155,154,169,170;,
  4;153,152,167,168;,
  4;164,163,178,179;,
  4;166,165,180,181;,
  4;177,176,191,192;,
  4;175,174,189,190;,
  4;173,172,187,188;,
  4;171,170,185,186;,
  4;169,168,183,184;,
  4;167,166,181,182;,
  4;178,177,192,193;,
  4;176,175,190,191;,
  4;174,173,188,189;,
  4;172,171,186,187;,
  4;170,169,184,185;,
  4;168,167,182,183;,
  4;179,178,193,194;,
  4;181,180,195,196;,
  4;192,191,206,207;,
  4;190,189,204,205;,
  4;188,187,202,203;,
  4;186,185,200,201;,
  4;184,183,198,199;,
  4;182,181,196,197;,
  4;193,192,207,208;,
  4;191,190,205,206;,
  4;189,188,203,204;,
  4;187,186,201,202;,
  4;185,184,199,200;,
  4;183,182,197,198;,
  4;194,193,208,209;,
  4;196,195,211,212;,
  4;207,206,222,223;,
  4;205,204,220,221;,
  4;203,202,218,219;,
  4;201,200,216,217;,
  4;199,198,214,215;,
  4;197,196,212,213;,
  4;208,207,223,224;,
  4;206,205,221,222;,
  4;204,203,219,220;,
  4;202,201,217,218;,
  4;200,199,215,216;,
  4;198,197,213,214;,
  4;209,208,224,225;,
  4;212,211,226,227;,
  4;223,222,237,238;,
  4;221,220,235,236;,
  4;219,218,233,234;,
  4;217,216,231,232;,
  4;215,214,229,230;,
  4;213,212,227,228;,
  4;224,223,238,239;,
  4;222,221,236,237;,
  4;220,219,234,235;,
  4;218,217,232,233;,
  4;216,215,230,231;,
  4;214,213,228,229;,
  4;225,224,239,240;,
  4;227,226,241,242;,
  4;238,237,252,253;,
  4;236,235,250,251;,
  4;234,233,248,249;,
  4;232,231,246,247;,
  4;230,229,244,245;,
  4;228,227,242,243;,
  4;239,238,253,254;,
  4;237,236,251,252;,
  4;235,234,249,250;,
  4;233,232,247,248;,
  4;231,230,245,246;,
  4;229,228,243,244;,
  4;240,239,254,255;,
  3;210,14,29;,
  3;0,256,15;,
  3;210,29,44;,
  3;15,256,30;,
  3;210,44,59;,
  3;30,256,45;,
  3;210,59,74;,
  3;45,256,60;,
  3;210,74,89;,
  3;60,256,75;,
  3;210,89,104;,
  3;75,256,90;,
  3;210,104,119;,
  3;90,256,105;,
  3;210,119,134;,
  3;105,256,120;,
  3;210,134,149;,
  3;120,256,135;,
  3;210,149,164;,
  3;135,256,150;,
  3;210,164,179;,
  3;150,256,165;,
  3;210,179,194;,
  3;165,256,180;,
  3;210,194,209;,
  3;180,256,195;,
  3;210,209,225;,
  3;195,256,211;,
  3;210,225,240;,
  3;211,256,226;,
  3;210,240,255;,
  3;226,256,241;,
  4;257,257,257,257;,
  4;258,258,258,258;,
  4;259,259,259,259;,
  4;260,260,260,260;,
  4;261,261,262,262;,
  4;263,263,264,264;,
  4;257,257,257,257;,
  4;262,262,265,265;,
  4;264,264,266,266;,
  4;259,259,259,259;;
 }
 MeshTextureCoords {
  291;
  0.306050;0.420940;,
  0.331730;0.409450;,
  0.329880;0.404360;,
  0.303080;0.410650;,
  0.044290;0.322040;,
  0.055550;0.346150;,
  0.070660;0.336030;,
  0.056940;0.315910;,
  0.070730;0.368780;,
  0.089610;0.389170;,
  0.105350;0.370570;,
  0.086880;0.354330;,
  0.111760;0.406630;,
  0.136660;0.420640;,
  0.147970;0.396150;,
  0.125800;0.384550;,
  0.163680;0.430800;,
  0.192150;0.436840;,
  0.196520;0.411730;,
  0.171620;0.405240;,
  0.221330;0.438660;,
  0.250490;0.436340;,
  0.249030;0.416560;,
  0.222420;0.415520;,
  0.278910;0.430220;,
  0.276040;0.414870;,
  0.036910;0.297300;,
  0.045930;0.294320;,
  0.032970;0.272670;,
  0.037630;0.271690;,
  0.329040;0.399080;,
  0.302010;0.400330;,
  0.083040;0.324930;,
  0.067630;0.308320;,
  0.118050;0.352890;,
  0.099900;0.339770;,
  0.157790;0.374240;,
  0.137370;0.364360;,
  0.201820;0.389360;,
  0.179280;0.382580;,
  0.250010;0.398160;,
  0.225400;0.394570;,
  0.275590;0.400080;,
  0.053870;0.289940;,
  0.041910;0.269880;,
  0.329200;0.393780;,
  0.302690;0.390200;,
  0.093100;0.313160;,
  0.076480;0.299690;,
  0.128430;0.335770;,
  0.110430;0.325120;,
  0.166540;0.353930;,
  0.147120;0.345320;,
  0.207810;0.368800;,
  0.186740;0.361730;,
  0.252960;0.380910;,
  0.229850;0.375190;,
  0.277230;0.385920;,
  0.060640;0.284470;,
  0.045690;0.267340;,
  0.330320;0.388630;,
  0.305000;0.380460;,
  0.101140;0.300900;,
  0.083610;0.290280;,
  0.136950;0.318900;,
  0.118900;0.310340;,
  0.174480;0.334440;,
  0.155450;0.326840;,
  0.214460;0.349320;,
  0.194120;0.341880;,
  0.257650;0.364540;,
  0.235590;0.356850;,
  0.280750;0.372420;,
  0.066210;0.278170;,
  0.048880;0.264220;,
  0.332370;0.383780;,
  0.308860;0.371250;,
  0.107350;0.288220;,
  0.089100;0.280300;,
  0.143840;0.301960;,
  0.125540;0.295320;,
  0.181820;0.315090;,
  0.162560;0.308420;,
  0.221850;0.330280;,
  0.201580;0.322310;,
  0.264020;0.348870;,
  0.242640;0.339110;,
  0.286070;0.359580;,
  0.070540;0.271220;,
  0.051410;0.260620;,
  0.335290;0.379380;,
  0.314220;0.362750;,
  0.111790;0.275180;,
  0.093020;0.269900;,
  0.149110;0.284680;,
  0.130380;0.279960;,
  0.188630;0.295170;,
  0.168470;0.289520;,
  0.230220;0.311100;,
  0.209300;0.302300;,
  0.272200;0.333720;,
  0.251210;0.321590;,
  0.293200;0.347470;,
  0.073650;0.263820;,
  0.053250;0.256680;,
  0.339000;0.375570;,
  0.321050;0.355150;,
  0.114500;0.261890;,
  0.095390;0.259200;,
  0.152630;0.266820;,
  0.133370;0.264320;,
  0.194900;0.273660;,
  0.172900;0.269600;,
  0.240000;0.291140;,
  0.217580;0.280900;,
  0.282460;0.319040;,
  0.261690;0.303950;,
  0.302250;0.336190;,
  0.075530;0.256120;,
  0.054360;0.252510;,
  0.343400;0.372500;,
  0.329290;0.348710;,
  0.115470;0.248430;,
  0.096220;0.248360;,
  0.153990;0.248470;,
  0.134500;0.248470;,
  0.200170;0.248240;,
  0.174970;0.248340;,
  0.251970;0.269780;,
  0.227030;0.256910;,
  0.295220;0.304910;,
  0.274740;0.285980;,
  0.313360;0.326000;,
  0.076170;0.248270;,
  0.054720;0.248230;,
  0.326220;0.144650;,
  0.338260;0.120280;,
  0.333750;0.117620;,
  0.317670;0.138830;,
  0.114670;0.234980;,
  0.095500;0.237510;,
  0.152820;0.230260;,
  0.133600;0.232610;,
  0.194650;0.223280;,
  0.173010;0.227440;,
  0.226720;0.239710;,
  0.251610;0.226730;,
  0.239400;0.205430;,
  0.217170;0.215860;,
  0.274330;0.210060;,
  0.294470;0.190330;,
  0.281170;0.176590;,
  0.260840;0.192270;,
  0.311760;0.168270;,
  0.300160;0.158650;,
  0.075570;0.240410;,
  0.054330;0.243940;,
  0.329870;0.114210;,
  0.310500;0.131800;,
  0.112100;0.221660;,
  0.093240;0.226780;,
  0.149460;0.212540;,
  0.130690;0.216980;,
  0.188480;0.201980;,
  0.168650;0.207690;,
  0.229470;0.185740;,
  0.208880;0.194720;,
  0.270510;0.162350;,
  0.250070;0.174950;,
  0.290690;0.147940;,
  0.073730;0.232680;,
  0.053200;0.239770;,
  0.326720;0.110160;,
  0.304750;0.123850;,
  0.107820;0.208600;,
  0.089440;0.216310;,
  0.144340;0.195320;,
  0.126010;0.201680;,
  0.181800;0.182280;,
  0.162870;0.188940;,
  0.221020;0.166940;,
  0.201200;0.175010;,
  0.261980;0.147720;,
  0.241280;0.157870;,
  0.283150;0.136390;,
  0.070650;0.225230;,
  0.051330;0.235820;,
  0.324380;0.105630;,
  0.300460;0.115160;,
  0.101820;0.195840;,
  0.084060;0.206220;,
  0.137680;0.178430;,
  0.119640;0.186690;,
  0.174670;0.163220;,
  0.155980;0.170680;,
  0.213610;0.148430;,
  0.193870;0.155810;,
  0.255290;0.132640;,
  0.234060;0.140700;,
  0.277410;0.124150;,
  0.066340;0.218210;,
  0.048750;0.232220;,
  0.322920;0.100750;,
  0.297660;0.105910;,
  0.094020;0.183410;,
  0.077060;0.196640;,
  0.129600;0.161670;,
  0.111500;0.171780;,
  0.167020;0.144090;,
  0.148030;0.152440;,
  0.207030;0.129590;,
  0.186640;0.136450;,
  0.250290;0.116970;,
  0.228190;0.123050;,
  0.273430;0.111280;,
  0.060780;0.211790;,
  0.045520;0.229090;,
  0.322390;0.095670;,
  0.296420;0.096250;,
  0.084260;0.171410;,
  0.068330;0.187750;,
  0.119720;0.144570;,
  0.101420;0.157010;,
  0.158760;0.124250;,
  0.138820;0.133630;,
  0.201170;0.109820;,
  0.179500;0.116270;,
  0.247000;0.100520;,
  0.223630;0.104510;,
  0.271260;0.097800;,
  0.053980;0.206160;,
  0.041690;0.226540;,
  0.322810;0.090540;,
  0.296840;0.086360;,
  0.072260;0.159920;,
  0.057730;0.179780;,
  0.107710;0.126820;,
  0.089000;0.142220;,
  0.149710;0.102970;,
  0.128070;0.113670;,
  0.196060;0.088460;,
  0.172350;0.094550;,
  0.245610;0.083110;,
  0.220550;0.084610;,
  0.271070;0.083760;,
  0.045960;0.201580;,
  0.037330;0.224710;,
  0.324210;0.085540;,
  0.299030;0.076460;,
  0.057590;0.149190;,
  0.045080;0.173090;,
  0.092680;0.107790;,
  0.073680;0.127560;,
  0.139630;0.079390;,
  0.115270;0.091740;,
  0.191850;0.064490;,
  0.165130;0.070580;,
  0.246510;0.064570;,
  0.219340;0.062830;,
  0.273130;0.069270;,
  0.036760;0.198340;,
  0.032570;0.223710;,
  0.031480;0.248350;,
  0.356370;0.396870;,
  0.348890;0.095720;,
  0.658910;0.123160;,
  0.658760;0.353760;,
  0.531200;0.377370;,
  0.531410;0.063400;,
  0.373580;0.953100;,
  0.168180;0.940960;,
  0.208410;0.547080;,
  0.386610;0.557610;,
  0.843740;0.087880;,
  0.843930;0.360260;,
  0.727920;0.339780;,
  0.727780;0.139720;,
  0.613250;0.558030;,
  0.791490;0.548150;,
  0.830270;0.942170;,
  0.624830;0.953560;,
  0.895890;0.535950;,
  0.945250;0.928790;,
  0.499200;0.955670;,
  0.499930;0.559860;,
  0.403670;0.353590;,
  0.403830;0.122980;,
  0.053250;0.927150;,
  0.104050;0.534500;,
  0.959770;0.139560;,
  0.959910;0.339620;;
 }
}
