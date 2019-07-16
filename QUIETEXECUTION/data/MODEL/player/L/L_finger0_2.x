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
 298;
 1.37666;-0.91604;-0.01972;,
 1.17161;-1.16590;-0.01972;,
 1.14920;-1.16590;-0.24718;,
 1.35032;-0.91604;-0.28718;,
 0.92175;-1.37095;-0.01972;,
 0.63669;-1.52332;-0.01972;,
 0.62456;-1.52332;-0.14282;,
 0.90415;-1.37095;-0.19843;,
 0.63669;1.52332;-0.01972;,
 0.92175;1.37095;-0.01972;,
 0.90415;1.37095;-0.19843;,
 0.62456;1.52332;-0.14282;,
 1.17161;1.16590;-0.01972;,
 1.37666;0.91604;-0.01972;,
 1.35032;0.91604;-0.28718;,
 1.14920;1.16590;-0.24718;,
 1.52903;0.63098;-0.01972;,
 1.62285;0.32167;-0.01972;,
 1.59178;0.32167;-0.33521;,
 1.49976;0.63098;-0.31690;,
 1.65454;0.00000;-0.01972;,
 1.62285;-0.32167;-0.01972;,
 1.59178;-0.32167;-0.33521;,
 1.62285;0.00000;-0.34139;,
 1.52903;-0.63098;-0.01972;,
 1.49976;-0.63098;-0.31690;,
 0.32738;1.61714;-0.01972;,
 0.32120;1.61714;-0.08248;,
 0.32738;-1.61714;-0.01972;,
 0.32120;-1.61714;-0.08248;,
 1.08286;-1.16590;-0.46589;,
 1.27230;-0.91604;-0.54436;,
 0.58866;-1.52332;-0.26119;,
 0.85202;-1.37095;-0.37027;,
 0.85202;1.37095;-0.37027;,
 0.58866;1.52332;-0.26119;,
 1.27230;0.91604;-0.54436;,
 1.08286;1.16590;-0.46589;,
 1.49976;0.32167;-0.63857;,
 1.41307;0.63098;-0.60267;,
 1.49976;-0.32167;-0.63857;,
 1.52903;0.00000;-0.65070;,
 1.41307;-0.63098;-0.60267;,
 0.30289;1.61714;-0.14282;,
 0.30289;-1.61714;-0.14282;,
 0.97512;-1.16590;-0.66746;,
 1.14561;-0.91604;-0.78138;,
 0.53035;-1.52332;-0.37027;,
 0.76737;-1.37095;-0.52864;,
 0.76737;1.37095;-0.52864;,
 0.53035;1.52332;-0.37027;,
 1.14561;0.91604;-0.78138;,
 0.97512;1.16590;-0.66746;,
 1.35032;0.32167;-0.91816;,
 1.27230;0.63098;-0.86603;,
 1.35032;-0.32167;-0.91816;,
 1.37666;0.00000;-0.93576;,
 1.27230;-0.63098;-0.86603;,
 0.27317;1.61714;-0.19843;,
 0.27317;-1.61714;-0.19843;,
 0.83012;-1.16590;-0.84413;,
 0.97512;-0.91604;-0.98913;,
 0.45188;-1.52332;-0.46589;,
 0.65345;-1.37095;-0.66746;,
 0.65345;1.37095;-0.66746;,
 0.45188;1.52332;-0.46589;,
 0.97512;0.91604;-0.98913;,
 0.83012;1.16590;-0.84413;,
 1.14920;0.32167;-1.16321;,
 1.08286;0.63098;-1.09687;,
 1.14920;-0.32167;-1.16321;,
 1.17161;0.00000;-1.18562;,
 1.08286;-0.63098;-1.09687;,
 0.23317;1.61714;-0.24718;,
 0.23317;-1.61714;-0.24718;,
 0.65345;-1.16590;-0.98913;,
 0.76737;-0.91604;-1.15962;,
 0.35626;-1.52332;-0.54436;,
 0.51463;-1.37095;-0.78138;,
 0.51463;1.37095;-0.78138;,
 0.35626;1.52332;-0.54436;,
 0.76737;0.91604;-1.15962;,
 0.65345;1.16590;-0.98913;,
 0.90415;0.32167;-1.36433;,
 0.85202;0.63098;-1.28631;,
 0.90415;-0.32167;-1.36433;,
 0.92175;0.00000;-1.39067;,
 0.85202;-0.63098;-1.28631;,
 0.18442;1.61714;-0.28718;,
 0.18442;-1.61714;-0.28718;,
 0.45188;-1.16590;-1.09687;,
 0.53035;-0.91604;-1.28631;,
 0.24718;-1.52332;-0.60267;,
 0.35626;-1.37095;-0.86603;,
 0.35626;1.37095;-0.86603;,
 0.24718;1.52332;-0.60267;,
 0.53035;0.91604;-1.28631;,
 0.45188;1.16590;-1.09687;,
 0.62456;0.32167;-1.51377;,
 0.58866;0.63098;-1.42708;,
 0.62456;-0.32167;-1.51377;,
 0.63669;0.00000;-1.54304;,
 0.58866;-0.63098;-1.42708;,
 0.12881;1.61714;-0.31690;,
 0.12881;-1.61714;-0.31691;,
 0.23316;-1.16590;-1.16321;,
 0.27317;-0.91604;-1.36433;,
 0.12881;-1.52332;-0.63857;,
 0.18442;-1.37095;-0.91816;,
 0.18442;1.37095;-0.91816;,
 0.12881;1.52332;-0.63857;,
 0.27317;0.91604;-1.36433;,
 0.23317;1.16590;-1.16321;,
 0.32120;0.32167;-1.60579;,
 0.30289;0.63098;-1.51377;,
 0.32120;-0.32167;-1.60579;,
 0.32738;0.00000;-1.63686;,
 0.30289;-0.63098;-1.51377;,
 0.06846;1.61714;-0.33521;,
 0.06846;-1.61714;-0.33521;,
 0.00571;-1.16590;-1.18562;,
 0.00571;-0.91604;-1.39067;,
 0.00571;-1.52332;-0.65070;,
 0.00571;-1.37095;-0.93576;,
 0.00571;1.37095;-0.93576;,
 0.00571;1.52332;-0.65070;,
 0.00571;0.91604;-1.39067;,
 0.00571;1.16590;-1.18562;,
 0.00571;0.32167;-1.63686;,
 0.00571;0.63098;-1.54304;,
 0.00571;-0.32167;-1.63686;,
 0.00571;0.00000;-1.66855;,
 0.00571;-0.63098;-1.54304;,
 0.00571;1.61714;-0.34139;,
 0.00571;-1.61714;-0.34139;,
 0.00571;-0.91604;-1.39067;,
 0.00571;-1.16590;-1.18562;,
 -0.22174;-1.16590;-1.16321;,
 -0.26175;-0.91604;-1.36433;,
 0.00571;-1.37095;-0.93576;,
 0.00571;-1.52332;-0.65070;,
 -0.11739;-1.52332;-0.63857;,
 -0.17300;-1.37095;-0.91816;,
 -0.17300;1.37095;-0.91816;,
 -0.11739;1.52332;-0.63857;,
 -0.26175;0.91604;-1.36433;,
 -0.22174;1.16590;-1.16321;,
 -0.30978;0.32167;-1.60579;,
 -0.29147;0.63098;-1.51377;,
 0.00571;-0.32167;-1.63686;,
 -0.30978;-0.32167;-1.60579;,
 -0.31596;0.00000;-1.63686;,
 0.00571;-0.63098;-1.54304;,
 -0.29147;-0.63098;-1.51377;,
 -0.05704;1.61714;-0.33521;,
 0.00571;-1.61714;-0.34139;,
 -0.05704;-1.61714;-0.33521;,
 -0.44046;-1.16590;-1.09687;,
 -0.51893;-0.91604;-1.28631;,
 -0.23575;-1.52332;-0.60267;,
 -0.34484;-1.37095;-0.86603;,
 -0.34484;1.37095;-0.86603;,
 -0.23575;1.52332;-0.60267;,
 -0.51893;0.91604;-1.28631;,
 -0.44046;1.16590;-1.09687;,
 -0.61314;0.32167;-1.51377;,
 -0.57724;0.63098;-1.42708;,
 -0.61314;-0.32167;-1.51377;,
 -0.62527;0.00000;-1.54304;,
 -0.57724;-0.63098;-1.42708;,
 -0.11739;1.61714;-0.31690;,
 -0.11739;-1.61714;-0.31691;,
 -0.64203;-1.16590;-0.98913;,
 -0.75595;-0.91604;-1.15962;,
 -0.34484;-1.52332;-0.54436;,
 -0.50321;-1.37095;-0.78138;,
 -0.50321;1.37095;-0.78138;,
 -0.34484;1.52332;-0.54436;,
 -0.75595;0.91604;-1.15962;,
 -0.64203;1.16590;-0.98913;,
 -0.89273;0.32167;-1.36433;,
 -0.84060;0.63098;-1.28631;,
 -0.89273;-0.32167;-1.36433;,
 -0.91033;0.00000;-1.39067;,
 -0.84060;-0.63098;-1.28631;,
 -0.17300;1.61714;-0.28718;,
 -0.17300;-1.61714;-0.28718;,
 -0.81870;-1.16590;-0.84413;,
 -0.96370;-0.91604;-0.98913;,
 -0.44046;-1.52332;-0.46589;,
 -0.64203;-1.37095;-0.66746;,
 -0.64203;1.37095;-0.66746;,
 -0.44046;1.52332;-0.46589;,
 -0.96370;0.91604;-0.98913;,
 -0.81870;1.16590;-0.84413;,
 -1.13778;0.32167;-1.16321;,
 -1.07144;0.63098;-1.09687;,
 -1.13778;-0.32167;-1.16321;,
 -1.16019;0.00000;-1.18562;,
 -1.07144;-0.63098;-1.09687;,
 -0.22174;1.61714;-0.24718;,
 -0.22175;-1.61714;-0.24718;,
 -0.96370;-1.16590;-0.66746;,
 -1.13419;-0.91604;-0.78138;,
 -0.51893;-1.52332;-0.37027;,
 -0.75595;-1.37095;-0.52864;,
 -0.75595;1.37095;-0.52864;,
 -0.51893;1.52332;-0.37027;,
 -1.13419;0.91604;-0.78138;,
 -0.96370;1.16590;-0.66746;,
 -1.33890;0.32167;-0.91816;,
 -1.26088;0.63098;-0.86603;,
 -1.33890;-0.32167;-0.91816;,
 -1.36524;0.00000;-0.93576;,
 -1.26088;-0.63098;-0.86603;,
 -0.26175;1.61714;-0.19843;,
 -0.26175;-1.61714;-0.19843;,
 -1.07144;-1.16590;-0.46589;,
 -1.26088;-0.91604;-0.54436;,
 -0.57724;-1.52332;-0.26119;,
 -0.84060;-1.37095;-0.37027;,
 -0.84060;1.37095;-0.37027;,
 -0.57724;1.52332;-0.26119;,
 -1.26088;0.91604;-0.54436;,
 -1.07144;1.16590;-0.46589;,
 -1.48834;0.32167;-0.63857;,
 -1.40165;0.63098;-0.60267;,
 -1.48834;-0.32167;-0.63857;,
 -1.51761;0.00000;-0.65070;,
 -1.40165;-0.63098;-0.60267;,
 -0.29147;1.61714;-0.14282;,
 -0.29147;-1.61714;-0.14282;,
 -1.13778;-1.16590;-0.24718;,
 -1.33890;-0.91604;-0.28718;,
 -0.61314;-1.52332;-0.14282;,
 -0.89273;-1.37095;-0.19843;,
 -0.89273;1.37095;-0.19843;,
 -0.61314;1.52332;-0.14282;,
 -1.33890;0.91604;-0.28718;,
 -1.13778;1.16590;-0.24718;,
 -1.58036;0.32167;-0.33521;,
 -1.48834;0.63098;-0.31690;,
 -1.58036;-0.32167;-0.33521;,
 -1.61143;0.00000;-0.34139;,
 -1.48834;-0.63098;-0.31690;,
 -0.30978;1.61714;-0.08248;,
 -0.30978;-1.61714;-0.08248;,
 -1.16019;-1.16590;-0.01972;,
 -1.36524;-0.91604;-0.01972;,
 -0.62527;-1.52332;-0.01972;,
 -0.91033;-1.37095;-0.01972;,
 -0.91033;1.37095;-0.01972;,
 -0.62527;1.52332;-0.01972;,
 -1.36524;0.91604;-0.01972;,
 -1.16019;1.16590;-0.01972;,
 -1.61143;0.32167;-0.01972;,
 -1.51761;0.63098;-0.01972;,
 -1.61143;-0.32167;-0.01972;,
 -1.64312;0.00000;-0.01972;,
 -1.51761;-0.63098;-0.01972;,
 -0.31596;1.61714;-0.01972;,
 -0.31596;-1.61714;-0.01972;,
 0.00571;1.64883;-0.01972;,
 0.00571;-1.64883;-0.01972;,
 0.00571;-1.64883;-0.01972;,
 1.76719;1.91721;0.00338;,
 1.76719;-1.58130;0.00338;,
 0.00300;-1.91904;0.00338;,
 0.00300;2.86227;0.00338;,
 -1.76120;1.91721;0.00338;,
 -1.76120;-1.58130;0.00338;,
 -1.57486;-1.04391;3.36709;,
 -1.57486;1.66864;3.36709;,
 0.00300;1.91826;5.34312;,
 0.00300;-0.53106;5.34312;,
 1.16243;-0.34620;5.34312;,
 1.16243;1.45277;5.34312;,
 1.58085;1.66864;3.36709;,
 1.58085;-1.04391;3.36709;,
 1.76719;-1.58130;0.00338;,
 1.76719;1.91721;0.00338;,
 0.00300;-1.31253;3.36709;,
 0.00300;-1.91904;0.00338;,
 0.00300;2.86227;0.00338;,
 0.00300;2.38902;3.36709;,
 -1.76120;-1.58130;0.00338;,
 -1.76120;1.91721;0.00338;,
 0.00300;-1.91904;0.00338;,
 0.00300;-1.31253;3.36709;,
 -1.15644;1.45277;5.34312;,
 -1.15644;-0.34620;5.34312;,
 1.16243;1.45277;5.34312;,
 1.16243;-0.34620;5.34312;,
 0.00300;-0.53106;5.34312;,
 -1.15644;-0.34620;5.34312;,
 0.00300;-0.53106;5.34312;,
 -1.15644;1.45277;5.34312;,
 0.00300;1.91826;5.34312;;
 
 272;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,0,3,25;,
 4;1,4,7,2;,
 4;26,8,11,27;,
 4;5,28,29,6;,
 4;9,12,15,10;,
 4;13,16,19,14;,
 4;17,20,23,18;,
 4;21,24,25,22;,
 4;3,2,30,31;,
 4;7,6,32,33;,
 4;11,10,34,35;,
 4;15,14,36,37;,
 4;19,18,38,39;,
 4;23,22,40,41;,
 4;25,3,31,42;,
 4;2,7,33,30;,
 4;27,11,35,43;,
 4;6,29,44,32;,
 4;10,15,37,34;,
 4;14,19,39,36;,
 4;18,23,41,38;,
 4;22,25,42,40;,
 4;31,30,45,46;,
 4;33,32,47,48;,
 4;35,34,49,50;,
 4;37,36,51,52;,
 4;39,38,53,54;,
 4;41,40,55,56;,
 4;42,31,46,57;,
 4;30,33,48,45;,
 4;43,35,50,58;,
 4;32,44,59,47;,
 4;34,37,52,49;,
 4;36,39,54,51;,
 4;38,41,56,53;,
 4;40,42,57,55;,
 4;46,45,60,61;,
 4;48,47,62,63;,
 4;50,49,64,65;,
 4;52,51,66,67;,
 4;54,53,68,69;,
 4;56,55,70,71;,
 4;57,46,61,72;,
 4;45,48,63,60;,
 4;58,50,65,73;,
 4;47,59,74,62;,
 4;49,52,67,64;,
 4;51,54,69,66;,
 4;53,56,71,68;,
 4;55,57,72,70;,
 4;61,60,75,76;,
 4;63,62,77,78;,
 4;65,64,79,80;,
 4;67,66,81,82;,
 4;69,68,83,84;,
 4;71,70,85,86;,
 4;72,61,76,87;,
 4;60,63,78,75;,
 4;73,65,80,88;,
 4;62,74,89,77;,
 4;64,67,82,79;,
 4;66,69,84,81;,
 4;68,71,86,83;,
 4;70,72,87,85;,
 4;76,75,90,91;,
 4;78,77,92,93;,
 4;80,79,94,95;,
 4;82,81,96,97;,
 4;84,83,98,99;,
 4;86,85,100,101;,
 4;87,76,91,102;,
 4;75,78,93,90;,
 4;88,80,95,103;,
 4;77,89,104,92;,
 4;79,82,97,94;,
 4;81,84,99,96;,
 4;83,86,101,98;,
 4;85,87,102,100;,
 4;91,90,105,106;,
 4;93,92,107,108;,
 4;95,94,109,110;,
 4;97,96,111,112;,
 4;99,98,113,114;,
 4;101,100,115,116;,
 4;102,91,106,117;,
 4;90,93,108,105;,
 4;103,95,110,118;,
 4;92,104,119,107;,
 4;94,97,112,109;,
 4;96,99,114,111;,
 4;98,101,116,113;,
 4;100,102,117,115;,
 4;106,105,120,121;,
 4;108,107,122,123;,
 4;110,109,124,125;,
 4;112,111,126,127;,
 4;114,113,128,129;,
 4;116,115,130,131;,
 4;117,106,121,132;,
 4;105,108,123,120;,
 4;118,110,125,133;,
 4;107,119,134,122;,
 4;109,112,127,124;,
 4;111,114,129,126;,
 4;113,116,131,128;,
 4;115,117,132,130;,
 4;135,136,137,138;,
 4;139,140,141,142;,
 4;125,124,143,144;,
 4;127,126,145,146;,
 4;129,128,147,148;,
 4;131,149,150,151;,
 4;152,135,138,153;,
 4;136,139,142,137;,
 4;133,125,144,154;,
 4;140,155,156,141;,
 4;124,127,146,143;,
 4;126,129,148,145;,
 4;128,131,151,147;,
 4;149,152,153,150;,
 4;138,137,157,158;,
 4;142,141,159,160;,
 4;144,143,161,162;,
 4;146,145,163,164;,
 4;148,147,165,166;,
 4;151,150,167,168;,
 4;153,138,158,169;,
 4;137,142,160,157;,
 4;154,144,162,170;,
 4;141,156,171,159;,
 4;143,146,164,161;,
 4;145,148,166,163;,
 4;147,151,168,165;,
 4;150,153,169,167;,
 4;158,157,172,173;,
 4;160,159,174,175;,
 4;162,161,176,177;,
 4;164,163,178,179;,
 4;166,165,180,181;,
 4;168,167,182,183;,
 4;169,158,173,184;,
 4;157,160,175,172;,
 4;170,162,177,185;,
 4;159,171,186,174;,
 4;161,164,179,176;,
 4;163,166,181,178;,
 4;165,168,183,180;,
 4;167,169,184,182;,
 4;173,172,187,188;,
 4;175,174,189,190;,
 4;177,176,191,192;,
 4;179,178,193,194;,
 4;181,180,195,196;,
 4;183,182,197,198;,
 4;184,173,188,199;,
 4;172,175,190,187;,
 4;185,177,192,200;,
 4;174,186,201,189;,
 4;176,179,194,191;,
 4;178,181,196,193;,
 4;180,183,198,195;,
 4;182,184,199,197;,
 4;188,187,202,203;,
 4;190,189,204,205;,
 4;192,191,206,207;,
 4;194,193,208,209;,
 4;196,195,210,211;,
 4;198,197,212,213;,
 4;199,188,203,214;,
 4;187,190,205,202;,
 4;200,192,207,215;,
 4;189,201,216,204;,
 4;191,194,209,206;,
 4;193,196,211,208;,
 4;195,198,213,210;,
 4;197,199,214,212;,
 4;203,202,217,218;,
 4;205,204,219,220;,
 4;207,206,221,222;,
 4;209,208,223,224;,
 4;211,210,225,226;,
 4;213,212,227,228;,
 4;214,203,218,229;,
 4;202,205,220,217;,
 4;215,207,222,230;,
 4;204,216,231,219;,
 4;206,209,224,221;,
 4;208,211,226,223;,
 4;210,213,228,225;,
 4;212,214,229,227;,
 4;218,217,232,233;,
 4;220,219,234,235;,
 4;222,221,236,237;,
 4;224,223,238,239;,
 4;226,225,240,241;,
 4;228,227,242,243;,
 4;229,218,233,244;,
 4;217,220,235,232;,
 4;230,222,237,245;,
 4;219,231,246,234;,
 4;221,224,239,236;,
 4;223,226,241,238;,
 4;225,228,243,240;,
 4;227,229,244,242;,
 4;233,232,247,248;,
 4;235,234,249,250;,
 4;237,236,251,252;,
 4;239,238,253,254;,
 4;241,240,255,256;,
 4;243,242,257,258;,
 4;244,233,248,259;,
 4;232,235,250,247;,
 4;245,237,252,260;,
 4;234,246,261,249;,
 4;236,239,254,251;,
 4;238,241,256,253;,
 4;240,243,258,255;,
 4;242,244,259,257;,
 3;262,26,27;,
 3;28,263,29;,
 3;262,27,43;,
 3;29,263,44;,
 3;262,43,58;,
 3;44,263,59;,
 3;262,58,73;,
 3;59,263,74;,
 3;262,73,88;,
 3;74,263,89;,
 3;262,88,103;,
 3;89,263,104;,
 3;262,103,118;,
 3;104,263,119;,
 3;262,118,133;,
 3;119,263,134;,
 3;262,133,154;,
 3;155,264,156;,
 3;262,154,170;,
 3;156,264,171;,
 3;262,170,185;,
 3;171,264,186;,
 3;262,185,200;,
 3;186,264,201;,
 3;262,200,215;,
 3;201,264,216;,
 3;262,215,230;,
 3;216,264,231;,
 3;262,230,245;,
 3;231,264,246;,
 3;262,245,260;,
 3;246,264,261;,
 4;265,266,267,268;,
 4;269,270,271,272;,
 4;273,274,275,276;,
 4;277,278,279,280;,
 4;279,278,281,282;,
 4;277,280,283,284;,
 4;268,267,285,286;,
 4;287,288,271,270;,
 4;284,283,269,272;,
 4;289,290,274,273;,
 4;291,292,278,277;,
 4;288,293,294,271;,
 4;278,292,295,281;,
 4;272,271,294,296;,
 4;297,284,272,296;,
 4;291,277,284,297;;
 
 MeshMaterialList {
  1;
  272;
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
    "data\\TEXTURE\\UV\\player\\L\\L_finger0_1_UV.jpg";
   }
  }
 }
 MeshNormals {
  283;
  0.195039;-0.980607;-0.019211;,
  0.382371;-0.923241;-0.037667;,
  0.554702;-0.830253;-0.054642;,
  0.705364;-0.705432;-0.069480;,
  0.828671;-0.553753;-0.081619;,
  0.920069;-0.381131;-0.090615;,
  0.976238;-0.194198;-0.096153;,
  0.995184;0.000000;-0.098025;,
  0.976238;0.194198;-0.096153;,
  0.920069;0.381131;-0.090615;,
  0.828671;0.553753;-0.081619;,
  0.705364;0.705432;-0.069480;,
  0.554703;0.830252;-0.054642;,
  0.382371;0.923241;-0.037667;,
  0.195039;0.980607;-0.019211;,
  0.191325;-0.980789;-0.038061;,
  0.375291;-0.923896;-0.074652;,
  0.544860;-0.831494;-0.108381;,
  0.693486;-0.707140;-0.137947;,
  0.815469;-0.555606;-0.162211;,
  0.906117;-0.382709;-0.180239;,
  0.961938;-0.195104;-0.191338;,
  0.980786;0.000000;-0.195086;,
  0.961938;0.195104;-0.191338;,
  0.906117;0.382709;-0.180239;,
  0.815469;0.555606;-0.162211;,
  0.693486;0.707140;-0.137947;,
  0.544860;0.831494;-0.108381;,
  0.375291;0.923896;-0.074652;,
  0.191325;0.980789;-0.038061;,
  0.180224;-0.980789;-0.074653;,
  0.353517;-0.923896;-0.146432;,
  0.513246;-0.831494;-0.212592;,
  0.653252;-0.707140;-0.270584;,
  0.768157;-0.555602;-0.318184;,
  0.853540;-0.382715;-0.353551;,
  0.906126;-0.195109;-0.375323;,
  0.923883;0.000000;-0.382675;,
  0.906126;0.195109;-0.375323;,
  0.853540;0.382715;-0.353551;,
  0.768157;0.555602;-0.318184;,
  0.653251;0.707140;-0.270584;,
  0.513246;0.831494;-0.212592;,
  0.353518;0.923895;-0.146432;,
  0.180224;0.980789;-0.074653;,
  0.162204;-0.980788;-0.108371;,
  0.318163;-0.923895;-0.212582;,
  0.461910;-0.831494;-0.308638;,
  0.587909;-0.707140;-0.392830;,
  0.691324;-0.555603;-0.461926;,
  0.768168;-0.382714;-0.513272;,
  0.815488;-0.195111;-0.544895;,
  0.831468;0.000000;-0.555573;,
  0.815488;0.195111;-0.544895;,
  0.768168;0.382714;-0.513272;,
  0.691324;0.555603;-0.461926;,
  0.587909;0.707141;-0.392830;,
  0.461910;0.831494;-0.308637;,
  0.318163;0.923895;-0.212582;,
  0.162204;0.980788;-0.108371;,
  0.137938;-0.980788;-0.137938;,
  0.270570;-0.923896;-0.270570;,
  0.392822;-0.831493;-0.392823;,
  0.499976;-0.707140;-0.499977;,
  0.587921;-0.555606;-0.587921;,
  0.653274;-0.382710;-0.653274;,
  0.693517;-0.195110;-0.693517;,
  0.707107;0.000000;-0.707107;,
  0.693517;0.195110;-0.693517;,
  0.653274;0.382710;-0.653274;,
  0.587921;0.555606;-0.587921;,
  0.499975;0.707141;-0.499976;,
  0.392821;0.831494;-0.392822;,
  0.270571;0.923896;-0.270571;,
  0.137938;0.980788;-0.137938;,
  0.108378;-0.980788;-0.162200;,
  0.212587;-0.923894;-0.318160;,
  0.308638;-0.831494;-0.461910;,
  0.392830;-0.707140;-0.587910;,
  0.461926;-0.555603;-0.691325;,
  0.513271;-0.382714;-0.768168;,
  0.544895;-0.195111;-0.815488;,
  0.555573;0.000000;-0.831468;,
  0.544895;0.195111;-0.815488;,
  0.513271;0.382714;-0.768168;,
  0.461926;0.555603;-0.691325;,
  0.392830;0.707141;-0.587909;,
  0.308637;0.831494;-0.461910;,
  0.212582;0.923895;-0.318162;,
  0.108370;0.980789;-0.162203;,
  0.074649;-0.980789;-0.180225;,
  0.146430;-0.923895;-0.353519;,
  0.212592;-0.831494;-0.513247;,
  0.270582;-0.707139;-0.653253;,
  0.318183;-0.555603;-0.768157;,
  0.353550;-0.382715;-0.853541;,
  0.375323;-0.195108;-0.906127;,
  0.382674;0.000000;-0.923883;,
  0.375323;0.195108;-0.906127;,
  0.353550;0.382715;-0.853540;,
  0.318184;0.555602;-0.768157;,
  0.270584;0.707139;-0.653252;,
  0.212592;0.831494;-0.513247;,
  0.146430;0.923896;-0.353517;,
  0.074650;0.980789;-0.180225;,
  0.038050;-0.980788;-0.191329;,
  0.074646;-0.923895;-0.375294;,
  0.108380;-0.831493;-0.544861;,
  0.137945;-0.707140;-0.693487;,
  0.162210;-0.555607;-0.815469;,
  0.180238;-0.382709;-0.906117;,
  0.191337;-0.195103;-0.961938;,
  0.195086;0.000000;-0.980786;,
  0.191337;0.195103;-0.961938;,
  0.180238;0.382709;-0.906117;,
  0.162211;0.555606;-0.815469;,
  0.137947;0.707140;-0.693487;,
  0.108381;0.831493;-0.544860;,
  0.074651;0.923896;-0.375291;,
  0.038059;0.980789;-0.191325;,
  0.000000;-0.980788;-0.195074;,
  0.000000;-0.923896;-0.382643;,
  -0.000000;-0.831494;-0.555534;,
  -0.000000;-0.707141;-0.707072;,
  0.000000;-0.555608;-0.831445;,
  0.000000;-0.382705;-0.923871;,
  0.000000;-0.195102;-0.980783;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.195102;-0.980783;,
  0.000000;0.382705;-0.923870;,
  -0.000000;0.555607;-0.831445;,
  -0.000001;0.707141;-0.707072;,
  -0.000000;0.831494;-0.555534;,
  -0.000000;0.923896;-0.382643;,
  0.000000;0.980788;-0.195074;,
  -0.038051;-0.980788;-0.191329;,
  -0.074648;-0.923895;-0.375293;,
  -0.108381;-0.831493;-0.544860;,
  -0.137945;-0.707141;-0.693486;,
  -0.162210;-0.555607;-0.815468;,
  -0.180238;-0.382709;-0.906117;,
  -0.191337;-0.195103;-0.961938;,
  -0.195086;0.000000;-0.980786;,
  -0.191337;0.195103;-0.961938;,
  -0.180238;0.382709;-0.906117;,
  -0.162210;0.555607;-0.815468;,
  -0.137945;0.707141;-0.693486;,
  -0.108381;0.831493;-0.544860;,
  -0.074653;0.923896;-0.375291;,
  -0.038060;0.980788;-0.191326;,
  -0.074649;-0.980788;-0.180226;,
  -0.146429;-0.923895;-0.353519;,
  -0.212590;-0.831495;-0.513247;,
  -0.270582;-0.707140;-0.653252;,
  -0.318183;-0.555603;-0.768157;,
  -0.353551;-0.382715;-0.853540;,
  -0.375323;-0.195108;-0.906126;,
  -0.382674;0.000000;-0.923883;,
  -0.375323;0.195108;-0.906126;,
  -0.353551;0.382715;-0.853540;,
  -0.318183;0.555603;-0.768157;,
  -0.270582;0.707140;-0.653252;,
  -0.212590;0.831495;-0.513247;,
  -0.146429;0.923895;-0.353519;,
  -0.074650;0.980788;-0.180227;,
  -0.108377;-0.980788;-0.162202;,
  -0.212584;-0.923894;-0.318162;,
  -0.308636;-0.831494;-0.461911;,
  -0.392831;-0.707139;-0.587910;,
  -0.461927;-0.555602;-0.691324;,
  -0.513272;-0.382714;-0.768168;,
  -0.544896;-0.195111;-0.815488;,
  -0.555573;0.000000;-0.831468;,
  -0.544896;0.195111;-0.815488;,
  -0.513272;0.382714;-0.768168;,
  -0.461927;0.555602;-0.691324;,
  -0.392830;0.707140;-0.587909;,
  -0.308635;0.831495;-0.461910;,
  -0.212581;0.923895;-0.318161;,
  -0.108374;0.980788;-0.162201;,
  -0.137939;-0.980788;-0.137939;,
  -0.270571;-0.923895;-0.270571;,
  -0.392822;-0.831493;-0.392822;,
  -0.499976;-0.707140;-0.499977;,
  -0.587921;-0.555605;-0.587921;,
  -0.653274;-0.382710;-0.653273;,
  -0.693517;-0.195110;-0.693517;,
  -0.707107;0.000000;-0.707107;,
  -0.693517;0.195110;-0.693517;,
  -0.653274;0.382710;-0.653274;,
  -0.587921;0.555605;-0.587921;,
  -0.499977;0.707140;-0.499976;,
  -0.392822;0.831493;-0.392822;,
  -0.270569;0.923896;-0.270570;,
  -0.137937;0.980789;-0.137939;,
  -0.162204;-0.980788;-0.108370;,
  -0.318162;-0.923895;-0.212582;,
  -0.461910;-0.831494;-0.308637;,
  -0.587909;-0.707140;-0.392830;,
  -0.691324;-0.555603;-0.461926;,
  -0.768168;-0.382714;-0.513272;,
  -0.815488;-0.195111;-0.544895;,
  -0.831468;0.000000;-0.555573;,
  -0.815488;0.195111;-0.544895;,
  -0.768168;0.382714;-0.513272;,
  -0.691324;0.555603;-0.461926;,
  -0.587910;0.707140;-0.392830;,
  -0.461911;0.831493;-0.308638;,
  -0.318159;0.923895;-0.212585;,
  -0.162198;0.980789;-0.108375;,
  -0.000000;0.998014;-0.062998;,
  -0.180224;-0.980789;-0.074653;,
  -0.353517;-0.923896;-0.146432;,
  -0.513247;-0.831494;-0.212592;,
  -0.653252;-0.707139;-0.270584;,
  -0.768157;-0.555602;-0.318184;,
  -0.853541;-0.382715;-0.353550;,
  -0.906127;-0.195108;-0.375323;,
  -0.923883;0.000000;-0.382674;,
  -0.906127;0.195108;-0.375323;,
  -0.853541;0.382715;-0.353550;,
  -0.768157;0.555602;-0.318184;,
  -0.653252;0.707140;-0.270584;,
  -0.513246;0.831495;-0.212592;,
  -0.353517;0.923896;-0.146432;,
  -0.180224;0.980789;-0.074653;,
  -0.191325;-0.980789;-0.038061;,
  -0.375291;-0.923896;-0.074652;,
  -0.544860;-0.831493;-0.108381;,
  -0.693486;-0.707141;-0.137946;,
  -0.815469;-0.555606;-0.162211;,
  -0.906117;-0.382709;-0.180239;,
  -0.961938;-0.195104;-0.191338;,
  -0.980786;0.000000;-0.195086;,
  -0.961938;0.195104;-0.191338;,
  -0.906117;0.382709;-0.180239;,
  -0.815469;0.555606;-0.162211;,
  -0.693486;0.707141;-0.137946;,
  -0.544859;0.831494;-0.108381;,
  -0.375291;0.923896;-0.074652;,
  -0.191325;0.980789;-0.038061;,
  -0.195039;-0.980607;-0.019210;,
  -0.382372;-0.923241;-0.037667;,
  -0.554702;-0.830253;-0.054642;,
  -0.705363;-0.705433;-0.069480;,
  -0.828671;-0.553754;-0.081619;,
  -0.920070;-0.381131;-0.090615;,
  -0.976238;-0.194198;-0.096153;,
  -0.995184;0.000000;-0.098025;,
  -0.976238;0.194198;-0.096153;,
  -0.920070;0.381131;-0.090615;,
  -0.828671;0.553754;-0.081619;,
  -0.705364;0.705432;-0.069480;,
  -0.554703;0.830252;-0.054642;,
  -0.382372;0.923240;-0.037667;,
  -0.195039;0.980607;-0.019210;,
  0.000000;-0.998014;-0.062997;,
  0.000000;0.000000;-1.000000;,
  -0.998469;0.000000;0.055312;,
  0.000000;-0.000000;1.000000;,
  0.991300;0.000000;0.131620;,
  -0.991300;0.000000;0.131620;,
  0.000001;-0.962798;0.270221;,
  0.000001;0.985667;0.168701;,
  0.998469;0.000000;0.055312;,
  0.175356;-0.969811;0.169461;,
  0.164337;-0.949708;0.266547;,
  0.000000;-0.985075;0.172128;,
  0.414399;0.897051;0.153534;,
  0.441409;0.890959;0.106539;,
  0.000001;0.992926;0.118732;,
  -0.164336;-0.949709;0.266547;,
  -0.175355;-0.969811;0.169461;,
  -0.441407;0.890960;0.106539;,
  -0.414397;0.897052;0.153534;,
  0.978308;0.000000;0.207155;,
  0.000001;-0.930927;0.365206;,
  -0.151681;-0.920155;0.360981;,
  0.151682;-0.920155;0.360981;,
  -0.978308;0.000000;0.207155;,
  0.000001;0.976195;0.216896;,
  -0.386154;0.900476;0.200072;,
  0.386156;0.900475;0.200072;;
  272;
  4;4,3,18,19;,
  4;2,1,16,17;,
  4;13,12,27,28;,
  4;11,10,25,26;,
  4;9,8,23,24;,
  4;7,6,21,22;,
  4;5,4,19,20;,
  4;3,2,17,18;,
  4;14,13,28,29;,
  4;1,0,15,16;,
  4;12,11,26,27;,
  4;10,9,24,25;,
  4;8,7,22,23;,
  4;6,5,20,21;,
  4;19,18,33,34;,
  4;17,16,31,32;,
  4;28,27,42,43;,
  4;26,25,40,41;,
  4;24,23,38,39;,
  4;22,21,36,37;,
  4;20,19,34,35;,
  4;18,17,32,33;,
  4;29,28,43,44;,
  4;16,15,30,31;,
  4;27,26,41,42;,
  4;25,24,39,40;,
  4;23,22,37,38;,
  4;21,20,35,36;,
  4;34,33,48,49;,
  4;32,31,46,47;,
  4;43,42,57,58;,
  4;41,40,55,56;,
  4;39,38,53,54;,
  4;37,36,51,52;,
  4;35,34,49,50;,
  4;33,32,47,48;,
  4;44,43,58,59;,
  4;31,30,45,46;,
  4;42,41,56,57;,
  4;40,39,54,55;,
  4;38,37,52,53;,
  4;36,35,50,51;,
  4;49,48,63,64;,
  4;47,46,61,62;,
  4;58,57,72,73;,
  4;56,55,70,71;,
  4;54,53,68,69;,
  4;52,51,66,67;,
  4;50,49,64,65;,
  4;48,47,62,63;,
  4;59,58,73,74;,
  4;46,45,60,61;,
  4;57,56,71,72;,
  4;55,54,69,70;,
  4;53,52,67,68;,
  4;51,50,65,66;,
  4;64,63,78,79;,
  4;62,61,76,77;,
  4;73,72,87,88;,
  4;71,70,85,86;,
  4;69,68,83,84;,
  4;67,66,81,82;,
  4;65,64,79,80;,
  4;63,62,77,78;,
  4;74,73,88,89;,
  4;61,60,75,76;,
  4;72,71,86,87;,
  4;70,69,84,85;,
  4;68,67,82,83;,
  4;66,65,80,81;,
  4;79,78,93,94;,
  4;77,76,91,92;,
  4;88,87,102,103;,
  4;86,85,100,101;,
  4;84,83,98,99;,
  4;82,81,96,97;,
  4;80,79,94,95;,
  4;78,77,92,93;,
  4;89,88,103,104;,
  4;76,75,90,91;,
  4;87,86,101,102;,
  4;85,84,99,100;,
  4;83,82,97,98;,
  4;81,80,95,96;,
  4;94,93,108,109;,
  4;92,91,106,107;,
  4;103,102,117,118;,
  4;101,100,115,116;,
  4;99,98,113,114;,
  4;97,96,111,112;,
  4;95,94,109,110;,
  4;93,92,107,108;,
  4;104,103,118,119;,
  4;91,90,105,106;,
  4;102,101,116,117;,
  4;100,99,114,115;,
  4;98,97,112,113;,
  4;96,95,110,111;,
  4;109,108,123,124;,
  4;107,106,121,122;,
  4;118,117,132,133;,
  4;116,115,130,131;,
  4;114,113,128,129;,
  4;112,111,126,127;,
  4;110,109,124,125;,
  4;108,107,122,123;,
  4;119,118,133,134;,
  4;106,105,120,121;,
  4;117,116,131,132;,
  4;115,114,129,130;,
  4;113,112,127,128;,
  4;111,110,125,126;,
  4;124,123,138,139;,
  4;122,121,136,137;,
  4;133,132,147,148;,
  4;131,130,145,146;,
  4;129,128,143,144;,
  4;127,126,141,142;,
  4;125,124,139,140;,
  4;123,122,137,138;,
  4;134,133,148,149;,
  4;121,120,135,136;,
  4;132,131,146,147;,
  4;130,129,144,145;,
  4;128,127,142,143;,
  4;126,125,140,141;,
  4;139,138,153,154;,
  4;137,136,151,152;,
  4;148,147,162,163;,
  4;146,145,160,161;,
  4;144,143,158,159;,
  4;142,141,156,157;,
  4;140,139,154,155;,
  4;138,137,152,153;,
  4;149,148,163,164;,
  4;136,135,150,151;,
  4;147,146,161,162;,
  4;145,144,159,160;,
  4;143,142,157,158;,
  4;141,140,155,156;,
  4;154,153,168,169;,
  4;152,151,166,167;,
  4;163,162,177,178;,
  4;161,160,175,176;,
  4;159,158,173,174;,
  4;157,156,171,172;,
  4;155,154,169,170;,
  4;153,152,167,168;,
  4;164,163,178,179;,
  4;151,150,165,166;,
  4;162,161,176,177;,
  4;160,159,174,175;,
  4;158,157,172,173;,
  4;156,155,170,171;,
  4;169,168,183,184;,
  4;167,166,181,182;,
  4;178,177,192,193;,
  4;176,175,190,191;,
  4;174,173,188,189;,
  4;172,171,186,187;,
  4;170,169,184,185;,
  4;168,167,182,183;,
  4;179,178,193,194;,
  4;166,165,180,181;,
  4;177,176,191,192;,
  4;175,174,189,190;,
  4;173,172,187,188;,
  4;171,170,185,186;,
  4;184,183,198,199;,
  4;182,181,196,197;,
  4;193,192,207,208;,
  4;191,190,205,206;,
  4;189,188,203,204;,
  4;187,186,201,202;,
  4;185,184,199,200;,
  4;183,182,197,198;,
  4;194,193,208,209;,
  4;181,180,195,196;,
  4;192,191,206,207;,
  4;190,189,204,205;,
  4;188,187,202,203;,
  4;186,185,200,201;,
  4;199,198,214,215;,
  4;197,196,212,213;,
  4;208,207,223,224;,
  4;206,205,221,222;,
  4;204,203,219,220;,
  4;202,201,217,218;,
  4;200,199,215,216;,
  4;198,197,213,214;,
  4;209,208,224,225;,
  4;196,195,211,212;,
  4;207,206,222,223;,
  4;205,204,220,221;,
  4;203,202,218,219;,
  4;201,200,216,217;,
  4;215,214,229,230;,
  4;213,212,227,228;,
  4;224,223,238,239;,
  4;222,221,236,237;,
  4;220,219,234,235;,
  4;218,217,232,233;,
  4;216,215,230,231;,
  4;214,213,228,229;,
  4;225,224,239,240;,
  4;212,211,226,227;,
  4;223,222,237,238;,
  4;221,220,235,236;,
  4;219,218,233,234;,
  4;217,216,231,232;,
  4;230,229,244,245;,
  4;228,227,242,243;,
  4;239,238,253,254;,
  4;237,236,251,252;,
  4;235,234,249,250;,
  4;233,232,247,248;,
  4;231,230,245,246;,
  4;229,228,243,244;,
  4;240,239,254,255;,
  4;227,226,241,242;,
  4;238,237,252,253;,
  4;236,235,250,251;,
  4;234,233,248,249;,
  4;232,231,246,247;,
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
  4;258,258,261,261;,
  4;259,259,259,259;,
  4;260,260,264,264;,
  4;265,266,262,267;,
  4;268,269,270,263;,
  4;257,257,257,257;,
  4;267,262,271,272;,
  4;263,270,273,274;,
  4;259,259,259,259;,
  4;275,275,260,260;,
  4;262,276,277,271;,
  4;266,278,276,262;,
  4;261,261,279,279;,
  4;280,263,274,281;,
  4;282,268,263,280;;
 }
 MeshTextureCoords {
  298;
  0.223890;0.472760;,
  0.255250;0.470470;,
  0.253820;0.449200;,
  0.225210;0.447890;,
  0.285850;0.464090;,
  0.315100;0.454300;,
  0.311980;0.443220;,
  0.282870;0.447570;,
  0.026590;0.319480;,
  0.034350;0.346130;,
  0.048000;0.339630;,
  0.036310;0.316340;,
  0.046290;0.372140;,
  0.062450;0.396580;,
  0.079920;0.381160;,
  0.062600;0.361360;,
  0.082610;0.418630;,
  0.106300;0.437560;,
  0.121550;0.413920;,
  0.099660;0.398740;,
  0.132970;0.452800;,
  0.161960;0.463910;,
  0.170670;0.436480;,
  0.145300;0.426540;,
  0.192520;0.470600;,
  0.197400;0.443630;,
  0.022530;0.292980;,
  0.027540;0.291960;,
  0.342790;0.442120;,
  0.340840;0.436640;,
  0.255000;0.429420;,
  0.228570;0.425390;,
  0.310900;0.432110;,
  0.282490;0.431660;,
  0.059540;0.331550;,
  0.044880;0.311690;,
  0.094020;0.365590;,
  0.075990;0.349520;,
  0.134130;0.392290;,
  0.113440;0.379820;,
  0.179070;0.412170;,
  0.156020;0.403060;,
  0.203250;0.419620;,
  0.032150;0.290040;,
  0.339970;0.430960;,
  0.258290;0.410890;,
  0.233480;0.404580;,
  0.311690;0.421230;,
  0.284360;0.416450;,
  0.069120;0.322330;,
  0.052200;0.305860;,
  0.105440;0.349910;,
  0.086890;0.336940;,
  0.144750;0.371890;,
  0.124720;0.361490;,
  0.187230;0.389800;,
  0.165560;0.381280;,
  0.209840;0.397560;,
  0.036240;0.287340;,
  0.340170;0.425260;,
  0.263450;0.393330;,
  0.239780;0.384910;,
  0.314240;0.410770;,
  0.288240;0.401950;,
  0.076850;0.312270;,
  0.058220;0.299120;,
  0.114650;0.334080;,
  0.095620;0.323800;,
  0.153830;0.352080;,
  0.134000;0.343410;,
  0.195310;0.368510;,
  0.174250;0.360380;,
  0.217120;0.376650;,
  0.039690;0.284000;,
  0.341420;0.419730;,
  0.270410;0.376520;,
  0.247490;0.365880;,
  0.318460;0.400900;,
  0.294050;0.388190;,
  0.082820;0.301580;,
  0.062930;0.291680;,
  0.121890;0.317980;,
  0.102380;0.310210;,
  0.161610;0.332320;,
  0.141520;0.325240;,
  0.203470;0.347530;,
  0.182270;0.339630;,
  0.225200;0.356240;,
  0.042440;0.280150;,
  0.343650;0.414530;,
  0.279320;0.360290;,
  0.256830;0.347100;,
  0.324290;0.391800;,
  0.301800;0.375220;,
  0.087110;0.290410;,
  0.066330;0.283750;,
  0.127210;0.301500;,
  0.107260;0.296230;,
  0.168090;0.312040;,
  0.147310;0.306710;,
  0.211910;0.326060;,
  0.189730;0.318260;,
  0.234330;0.335680;,
  0.044440;0.275930;,
  0.346820;0.409820;,
  0.290450;0.344580;,
  0.268220;0.328210;,
  0.331690;0.383680;,
  0.311600;0.363160;,
  0.089740;0.278930;,
  0.068400;0.275480;,
  0.130530;0.284700;,
  0.110260;0.281950;,
  0.172990;0.290660;,
  0.151220;0.287520;,
  0.220950;0.303110;,
  0.196620;0.295170;,
  0.244990;0.314290;,
  0.045670;0.271450;,
  0.350830;0.405750;,
  0.304260;0.329480;,
  0.282380;0.308980;,
  0.340590;0.376800;,
  0.323610;0.352280;,
  0.090700;0.267280;,
  0.069140;0.267040;,
  0.131860;0.267670;,
  0.111400;0.267500;,
  0.175370;0.267820;,
  0.152810;0.267800;,
  0.231290;0.277390;,
  0.202460;0.267880;,
  0.258000;0.291400;,
  0.046090;0.266850;,
  0.355590;0.402490;,
  0.282470;0.227350;,
  0.304260;0.206280;,
  0.290060;0.191410;,
  0.268080;0.208130;,
  0.323010;0.182680;,
  0.338710;0.157390;,
  0.329560;0.151060;,
  0.310600;0.172260;,
  0.090010;0.255610;,
  0.068550;0.258590;,
  0.131000;0.250610;,
  0.110630;0.253020;,
  0.173410;0.245320;,
  0.151680;0.248220;,
  0.231070;0.258900;,
  0.220970;0.233180;,
  0.196700;0.241000;,
  0.257920;0.245110;,
  0.244940;0.222130;,
  0.045700;0.262240;,
  0.351830;0.131270;,
  0.347000;0.128380;,
  0.278690;0.176020;,
  0.256620;0.189430;,
  0.321900;0.143460;,
  0.300490;0.160670;,
  0.087650;0.244060;,
  0.066630;0.250260;,
  0.127980;0.233780;,
  0.107960;0.238690;,
  0.168860;0.224060;,
  0.148200;0.229140;,
  0.212200;0.210400;,
  0.190220;0.218060;,
  0.234410;0.200880;,
  0.044500;0.257740;,
  0.342850;0.124680;,
  0.269630;0.160230;,
  0.247300;0.171010;,
  0.315780;0.134870;,
  0.292460;0.148210;,
  0.083630;0.232770;,
  0.063370;0.242230;,
  0.123060;0.217300;,
  0.103450;0.224610;,
  0.162770;0.203860;,
  0.142810;0.210600;,
  0.204090;0.189160;,
  0.183170;0.196840;,
  0.225450;0.180620;,
  0.042520;0.253490;,
  0.339490;0.120310;,
  0.262540;0.143970;,
  0.239650;0.152490;,
  0.311220;0.125500;,
  0.286380;0.135010;,
  0.077920;0.221890;,
  0.058780;0.234650;,
  0.116320;0.201140;,
  0.097090;0.210850;,
  0.155500;0.184180;,
  0.135760;0.192380;,
  0.196330;0.168450;,
  0.175640;0.176290;,
  0.217620;0.160660;,
  0.039780;0.249600;,
  0.337010;0.115420;,
  0.257270;0.127090;,
  0.233460;0.133470;,
  0.308280;0.115530;,
  0.282190;0.121140;,
  0.070460;0.211540;,
  0.052850;0.227710;,
  0.107660;0.185050;,
  0.088790;0.197440;,
  0.147080;0.164510;,
  0.127190;0.174300;,
  0.188700;0.147590;,
  0.167560;0.155670;,
  0.210670;0.140350;,
  0.036330;0.246210;,
  0.335480;0.110160;,
  0.253840;0.109390;,
  0.228700;0.113510;,
  0.307010;0.105140;,
  0.279950;0.106630;,
  0.061130;0.201920;,
  0.045580;0.221620;,
  0.096930;0.169100;,
  0.078380;0.184460;,
  0.137310;0.144230;,
  0.116690;0.155860;,
  0.181160;0.125850;,
  0.158810;0.134280;,
  0.204510;0.119060;,
  0.032220;0.243440;,
  0.334940;0.104690;,
  0.252470;0.090650;,
  0.225520;0.092100;,
  0.307530;0.094500;,
  0.279850;0.091540;,
  0.049800;0.193280;,
  0.036990;0.216640;,
  0.083680;0.153110;,
  0.065550;0.172030;,
  0.125890;0.122690;,
  0.103900;0.136680;,
  0.173630;0.102440;,
  0.149230;0.111340;,
  0.199170;0.096060;,
  0.027550;0.241440;,
  0.335430;0.099190;,
  0.253580;0.070730;,
  0.224370;0.068670;,
  0.309950;0.083880;,
  0.282160;0.075970;,
  0.036250;0.185990;,
  0.027120;0.213080;,
  0.067310;0.137240;,
  0.049860;0.160380;,
  0.112270;0.099020;,
  0.087880;0.116120;,
  0.166030;0.076620;,
  0.138550;0.085910;,
  0.194800;0.070270;,
  0.022440;0.240340;,
  0.336960;0.093820;,
  0.021090;0.266820;,
  0.369370;0.428780;,
  0.363430;0.104940;,
  0.661030;0.151970;,
  0.660180;0.408570;,
  0.530710;0.432920;,
  0.531850;0.082220;,
  0.375770;0.929370;,
  0.164190;0.875740;,
  0.252530;0.696380;,
  0.401400;0.729770;,
  0.843720;0.133600;,
  0.843720;0.397800;,
  0.718650;0.377860;,
  0.718650;0.183810;,
  0.598780;0.730490;,
  0.747900;0.698180;,
  0.834930;0.878170;,
  0.622970;0.930270;,
  0.829360;0.647620;,
  0.931340;0.827770;,
  0.499330;0.939860;,
  0.500060;0.737890;,
  0.401390;0.407720;,
  0.402230;0.151120;,
  0.068150;0.824640;,
  0.171440;0.645230;,
  0.968780;0.183810;,
  0.968780;0.377860;,
  0.577620;0.612090;,
  0.687090;0.585010;,
  0.254190;0.541340;,
  0.314150;0.583660;,
  0.747350;0.543120;,
  0.423430;0.611530;,
  0.500500;0.617220;;
 }
}
