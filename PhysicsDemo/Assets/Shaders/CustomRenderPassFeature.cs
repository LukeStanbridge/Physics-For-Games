using Palmmedia.ReportGenerator.Core;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Rendering.Universal;

public class CustomRenderPassFeature : ScriptableRendererFeature
{
    class CustomRenderPass : ScriptableRenderPass
    {
        public RenderTargetIdentifier source;
        private Material material;
        private RenderTargetHandle tempRenderHandle;

        public CustomRenderPass(Material material)
        {
            this.material = material;
            tempRenderHandle.Init("_TemporaryColorTexture");
        }

        public override void OnCameraSetup(CommandBuffer cmd, ref RenderingData renderingData)
        {
        }

        public override void Execute(ScriptableRenderContext context, ref RenderingData renderingData)
        {
            CommandBuffer commandBuffer = CommandBufferPool.Get();

            commandBuffer.GetTemporaryRT(tempRenderHandle.id, renderingData.cameraData.cameraTargetDescriptor);
            Blit(commandBuffer, source, tempRenderHandle.Identifier(), material);
            Blit(commandBuffer, tempRenderHandle.Identifier(), source);

            context.ExecuteCommandBuffer(commandBuffer);
            CommandBufferPool.Release(commandBuffer);
        }

        public override void OnCameraCleanup(CommandBuffer cmd)
        {
        }
    }

    [System.Serializable]

    public class Settings
    {
        public Material material = null;
    }

    public Settings settings = new Settings();

    CustomRenderPass m_ScriptablePass;

    public override void Create()
    {
        m_ScriptablePass = new CustomRenderPass(settings.material);

        m_ScriptablePass.renderPassEvent = RenderPassEvent.BeforeRenderingPostProcessing;
    }

    public override void AddRenderPasses(ScriptableRenderer renderer, ref RenderingData renderingData)
    {
        m_ScriptablePass.source = renderer.cameraColorTarget;
        renderer.EnqueuePass(m_ScriptablePass);
    }
}


