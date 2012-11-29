// CamelotClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CmApplication.h"
#include "CmDynLibManager.h"

#include "CmGameObject.h"
#include "CmCamera.h"
#include "CmHighLevelGpuProgramManager.h"
#include "CmRenderSystemManager.h"
#include "CmRenderSystem.h"
#include "CmRenderWindow.h"
#include "CmResources.h"
#include "CmRenderable.h"
#include "CmMaterial.h"
#include "CmShader.h"
#include "CmTechnique.h"
#include "CmPass.h"
#include "CmImporter.h"
#include "CmMesh.h"

#include "CmDebugCamera.h"

using namespace CamelotEngine;

int _tmain(int argc, _TCHAR* argv[])
{
	//gApplication().startUp("CamelotGLRenderSystem", "CamelotForwardRenderer");
	gApplication().startUp("CamelotD3D9RenderSystem", "CamelotForwardRenderer");

	RenderSystem* renderSystem = RenderSystemManager::getActive();
	RenderWindow* renderWindow = gApplication().getPrimaryRenderWindow();

	GameObjectPtr cameraGO = GameObject::create("MainCamera");
	CameraPtr camera = cameraGO->addComponent<Camera>();

	camera->init(renderWindow, 0.0f, 0.0f, 1.0f, 1.0f, 0);
	cameraGO->setPosition(Vector3(0,50,1240));
	cameraGO->lookAt(Vector3(0,50,-300));
	camera->setNearClipDistance(5);
	camera->setAspectRatio(800.0f / 600.0f);

	std::shared_ptr<DebugCamera> debugCamera = cameraGO->addComponent<DebugCamera>();

	GameObjectPtr testModelGO = GameObject::create("TestMesh");
	RenderablePtr testRenderable = testModelGO->addComponent<Renderable>();

	HighLevelGpuProgramPtr fragProg;
	HighLevelGpuProgramPtr vertProg;

	/////////////////// HLSL SHADERS //////////////////////////
	//String fragShaderCode = "sampler2D tex;			\
	//						float4 ps_main(float2 uv : TEXCOORD0) : COLOR0		\
	//						{														\
	//						float4 color = tex2D(tex, uv);				\
	//						return color;										\
	//						}";

	//fragProg =  HighLevelGpuProgramManager::instance().createProgram(fragShaderCode, "ps_main", "hlsl", GPT_FRAGMENT_PROGRAM, GPP_PS_2_0);
	//fragProg->load();

	//String vertShaderCode = "float4x4 matViewProjection;	\
	//						void vs_main(										\
	//						float4 inPos : POSITION,							\
	//						float2 uv : TEXCOORD0,								\
	//						out float4 oPosition : POSITION,					\
	//						out float2 oUv : TEXCOORD0)							\
	//						{														\
	//						oPosition = mul(matViewProjection, inPos);			\
	//						oUv = uv;											\
	//						}";

	//vertProg =  HighLevelGpuProgramManager::instance().createProgram(vertShaderCode, "vs_main", "hlsl", GPT_VERTEX_PROGRAM, GPP_VS_2_0);
	//vertProg->load();



	/////////////////// CG SHADERS //////////////////////////
	String fragShaderCode = "sampler2D tex;					\
							float4 ps_main(float2 uv : TEXCOORD0) : COLOR0		\
							{														\
							float4 color = tex2D(tex, uv);						\
							return color;										\
							}";

	fragProg =  HighLevelGpuProgramManager::instance().createProgram(fragShaderCode, "ps_main", "cg", GPT_FRAGMENT_PROGRAM, GPP_PS_2_0);
	fragProg->init();

	String vertShaderCode = "float4x4 matViewProjection;	\
							void vs_main(										\
							float4 inPos : POSITION,							\
							float2 uv : TEXCOORD0,								\
							out float4 oPosition : POSITION,					\
							out float2 oUv : TEXCOORD0)							\
							{														\
							oPosition = mul(matViewProjection, inPos);			\
							oUv = uv;											\
							}";

	vertProg =  HighLevelGpuProgramManager::instance().createProgram(vertShaderCode, "vs_main", "cg", GPT_VERTEX_PROGRAM, GPP_VS_2_0);
	vertProg->init();

	HighLevelGpuProgramRef vertProgRef(vertProg);

	gResources().create(vertProgRef, "C:\\vertProgCg.vprog", true);
	vertProgRef = static_resource_cast<HighLevelGpuProgram>(gResources().load("C:\\vertProgCg.vprog"));

	HighLevelGpuProgramRef fragProgRef(fragProg);

	gResources().create(fragProgRef, "C:\\fragProgCg.vprog", true);
	fragProgRef = static_resource_cast<HighLevelGpuProgram>(gResources().load("C:\\fragProgCg.vprog"));

	///////////////// GLSL SHADERS ////////////////////////////
	//String fragShaderCode = "uniform sampler2D tex; \
	//							void main() \
	//							  {\
	//							  vec4 texColor = texture2D(tex,gl_TexCoord[0].st);\
	//							  gl_FragColor = texColor; \
	//							  }";

	//fragProg = HighLevelGpuProgramManager::instance().createProgram(fragShaderCode, "main", "glsl", GPT_FRAGMENT_PROGRAM, GPP_PS_2_0);
	//fragProg->load();

	//// TODO - Ogres GLSL parsing requires some strict parameter naming, can that be avoided?
	//String vertShaderCode = "uniform mat4 matViewProjection; \
	//							  attribute vec4 vertex; \
	//						void main() \
	//							  { \
	//							  gl_TexCoord[0] = gl_MultiTexCoord0; \
	//							  gl_Position = matViewProjection * vertex; \
	//							  }";

	//vertProg = HighLevelGpuProgramManager::instance().createProgram(vertShaderCode, "main", "glsl", GPT_VERTEX_PROGRAM, GPP_VS_2_0);
	//vertProg->load();

	ShaderPtr testShader = ShaderPtr(new Shader("TestShader"));
	TechniquePtr newTechniqueGL = testShader->addTechnique("GLRenderSystem", "ForwardRenderer");
	PassPtr newPassGL = newTechniqueGL->addPass();
	newPassGL->setVertexProgram(vertProgRef);
	newPassGL->setFragmentProgram(fragProgRef);


	// TODO - I need to create different techniques for different render systems (and renderers, if there were any),
	// which is redundant as some techniques can be reused. I should add a functionality that supports multiple
	// render systems/renderers per technique
	TechniquePtr newTechniqueDX = testShader->addTechnique("D3D9RenderSystem", "ForwardRenderer");
	PassPtr newPassDX = newTechniqueDX->addPass();
	newPassDX->setVertexProgram(vertProgRef);
	newPassDX->setFragmentProgram(fragProgRef);

	MaterialRef testMaterial = MaterialPtr(new Material());
	testMaterial->setShader(testShader);

	/*TextureRef testTex = static_resource_cast<Texture>(Importer::instance().import("C:\\ImportTest.tga"));*/
	TextureRef testTex = static_resource_cast<Texture>(Importer::instance().import("C:\\ArenaTowerDFS.psd"));
	MeshRef dbgMesh = static_resource_cast<Mesh>(Importer::instance().import("C:\\X_Arena_Tower.FBX"));

	//int tmpFlag = _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_DELAY_FREE_MEM_DF);

	gResources().create(testTex, "C:\\ExportTest.tex", true);
	gResources().create(dbgMesh, "C:\\ExportMesh.mesh", true);

	testTex = static_resource_cast<Texture>(gResources().load("C:\\ExportTest.tex"));
	dbgMesh = static_resource_cast<Mesh>(gResources().load("C:\\ExportMesh.mesh"));

	testMaterial->setTexture("tex", testTex);
	gResources().create(testMaterial, "C:\\ExportMaterial.mat", true);

	testMaterial = gResources().load("C:\\ExportMaterial.mat");

	//_ASSERT(_CrtCheckMemory());

	testRenderable->setMesh(dbgMesh);
	testRenderable->setMaterial(testMaterial);

	//// Set the new state for the flag
	//_CrtSetDbgFlag( tmpFlag );

	gApplication().runMainLoop();

	gApplication().shutDown();

	return 0;
}

