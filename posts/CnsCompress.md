**\[Preview\] Have computers do most of central nervous system, such as thalamus, auditory cortex, visual cortices, homunculus**

Howto process *1024\*1280@60fps* (*standard definition*, *2.6 gigabits-per-second*) into compressed structures (such as text) which just use a *few kilobits-per-second*, then reproduce the originals from thus compressed structures (with small [perceptual losses](https://deepai.org/machine-learning-glossary-and-terms/perceptual-loss-function).)

\[[This post](./CnsCompress.md) \[[<sup>2</sup>](https://swudususuwu.substack.com/p/future-plans-have-computers-do-most)\] is [released through *Creative Commons Generic Attribution 2* (which allows all uses)](https://creativecommons.org/licenses/by/2.0/).\] \[This post is a work-in-progress.\]

# Table of Contents
- [Discussion](#Discussion)
  - [Compression plus synthesis](#Compression-plus-synthesis)
- [Synopsis + similar posts](#synopsis--similar-posts)

******************************************
# Discussion
For the most new sources, use programs such as [_iSH_](https://apps.apple.com/us/app/ish-shell/id1436902243) (for _iOS_) or [_Termux_](https://play.google.com/store/apps/details?id=com.termux) (for _Android OS_) to run this:
```
git clone https://github.com/SwuduSusuwu/SusuLib.git
cd ./SusuLib/cxx && git switch preview && git pull --rebase && ls
```
To [contribute](https://github.com/SwuduSusuwu/SusuLib?tab=readme-ov-file#how-to-contribute) to this, submit [new pull requests](https://github.com/SwuduSusuwu/SusuLib/pulls) which reference <https://github.com/SwuduSusuwu/SusuLib/issues/2>. Progress:
- [`cxx/ClassResultList.cxx`](https://github.com/SwuduSusuwu/SusuLib/blob/preview/cxx/ClassResultList.cxx) has correspondances to [neocortex](https://wikipedia.org/wiki/Neocortex). which is what humans use as databases.
- [`cxx/AssistantCns.cxx`](https://github.com/SwuduSusuwu/SusuLib/blob/preview/cxx/AssistantCns.cxx) uses correspondances to [_Broca's area_](https://wikipedia.org/wiki/Broca's_area) (produces language through recursive processes), [_Wernicke’s area_](https://wikipedia.org/wiki/Wernicke's_area) (parses languages through [recursive](https://wikipedia.org/wiki/recursion) processes), plus [hippocampus](https://wikipedia.org/wiki/Hippocampus) (integration to the neocortex + [imagination](https://wikipedia.org/wiki/Procedural_generation) through numerous regions).
- [`cxx/ClassCns.cxx`](https://github.com/SwuduSusuwu/SusuLib/blob/preview/cxx/ClassCns.cxx) (which can use "backends" (implementations) such as [`tensorflow`](https://github.com/tensorflow/tensorflow), [`apxr_run`](https://github.com/Rober-t/apxr_run) or [`HSOM`](https://github.com/CarsonScott/HSOM)) is a [pure-virtual](https://en.cppreference.com/book/intro/abstract_classes) general-purpose [emulations (heuristic approximates)](https://wikipedia.org/wiki/emulation) of neural tissue.
  - [`cxx/ClassTensorFlowCns.cxx`](https://github.com/SwuduSusuwu/SusuLib/blob/preview/cxx/ClassTensorFlowCns.cxx) implements `class Cns` through *TensorFlow* (which can use [*SIMD* + *GPGPU* + *TPU*](./SimdGpgpuTpu.md)).

<https://www.deviantart.com/dreamup> has some equivalences to how [visual cortex](https://wikipedia.org/wiki/Visual_cortex) + _Broca's area_ + hippocampus + text inputs = [texture generation](https://wikipedia.org/wiki/Procedural_generation) + [mesh generation](https://wikipedia.org/wiki/Mesh_generation) outputs.

To [have autonomous robots produce all goods for us](https://github.com/SwuduSusuwu/SusuPosts/blob/preview/posts/Autonomous-tools_+_human-consciousness.md) requires [visual cortex](https://wikipedia.org/wiki/Visual_cortex) (parses inputs from [photoreceptors](https://wikipedia.org/wiki/Photoreceptors)) + [auditory cortex](https://wikipedia.org/wiki/Auditory_cortex) (parses inputs from [malleus](https://wikipedia.org/wiki/Malleus) + [cortical homunculus](https://wikipedia.org/wiki/Cortical_homunculus) (parses inputs from touch sensors) + [thalamus](https://wikipedia.org/wiki/Thalamus) (merges information from numerous classes of sensors, thus the robot balances + produces [maps](https://wikipedia.org/wiki/Map_(mathematics))) + [hippocampus](https://wikipedia.org/wiki/Hippocampus) (uses outputs from sensors to setup neocortex, plus, runs [inverses](https://wikipedia.org/wiki/Inverse_function) this for synthesis of new scenarios) + *Wernicke's region*/*Broca's region*s (recursive [language processes](https://wikipedia.org/wiki/Natural_language_processing)).

Just as human central nervous systems which view *YouTube* use numerous steps:
- [Retinal nervous tissue](https://wikipedia.org/wiki/Retina) has raw [photons](https://wikipedia.org/wiki/Photons) as inputs; [compresses](https://wikipedia.org/wiki/Visual_system) those into splines + edges + motion vectors (close to how computers produce [splines](https://wikipedia.org/wiki/splines) through [edge detection](https://wikipedia.org/wiki/edge_detection) plus do [motion estimation](https://wikipedia.org/wiki/motion_estimation), which is what the most advanced traditional codecs such as [`x264`](https://wikipedia.org/wiki/x264) do to compress).
- Sends millions/billions of those (through optic nerves) to the *V1 visual cortex* (as opposed to just dump those to the human-version of filesystem storage, which is what computers do), which groups those to produce more [abstract](https://wikipedia.org/wiki/abstraction), [sparse](https://wikipedia.org/wiki/sparse_network), [compressed](https://wikipedia.org/wiki/compress) forms (close to a simulator's [meshes](https://wikipedia.org/wiki/mesh_generation) / [textures](https://wikipedia.org/wiki/image_textures) / [animations](https://wikipedia.org/wiki/animations)),
passes those to V1 [visual cortex](https://wikipedia.org/wiki/Visual_cortex),
which synthesizes those into more symbolic info (such as a simulator's specific instances of individual humans, tools, or houses).
- Sends the most symbolic / conceptual (from *V2 visual cortex*) plus complex (from *V1 visual cortex*) to [hippocampus](https://wikipedia.org/wiki/Hippocampus) (which performs temporary storage tasks while active, plus at rest encodes this into neocortices).
Just as human [neocortices](https://wikipedia.org/wiki/Neocortex) stored symbolic+conceptual resources have use for synthesis of new sounds+visuals, so too can artificial neural tissue (executed on [**CPU**](https://wikipedia.org/wiki/CPU) or [**GPU**](https://wikipedia.org/wiki/GPU)) setup [synapses](https://wikipedia.org/wiki/synapses) to allow to compress gigabytes of visuals from videos into a few kilobytes of "text" (symbolic + conceptual info). The hippocampus can also use the compressed "text" to reproduce sounds + visuals.

2 routes to this:
- [Unsupervised CNS](https://wikipedia.org/wiki/Unsupervised_learning) (fitness function of synapses is just to compress as much as can, plus reproduce as much of originals as can for us; layout of synapses is somewhat based on human CNS). This allows to insert a few paragraphs of text past the finish so this synthesizes hours of extra video for you.
- [Supervised CNS](https://wikipedia.org/wiki/Supervised_learning) (numerous [sub](https://wikipedia.org/wiki/Submodule)-CNS's for numerous stages of compression, with examples used to setup the synapses for those numerous stages to compress, such as "[raw](https://wikipedia.org/wiki/Binary_encoding) [bitmap](https://wikipedia.org/wiki/Bitmap) -> [Scalable Vector Graphics](https://wikipedia.org/wiki/Scalable_Vector_Graphics) + partial [texture](https://wikipedia.org/wiki/Image_texture) [synthesis](https://wikipedia.org/wiki/Procedural_generation)", "[video](https://wikipedia.org/wiki/Video) (vector of bitmaps) -> [motion estimation](https://wikipedia.org/wiki/Motion_estimation) vectors", "Scalable Vector Graphics/textures + [motion estimation](https://wikipedia.org/wiki/Motion_estimation) [vectors](https://wikipedia.org/wiki/Vector_calculus) -> [mesh generation](https://wikipedia.org/wiki/Mesh_generation) + [animation](https://wikipedia.org/wiki/Animation) + full [texture synthesis](https://wikipedia.org/wiki/Procedural_generation)", plus the [inverses](https://wikipedia.org/wiki/Inverse_function) to [decompress](https://wikipedia.org/wiki/Decompress)). This allows to insert a few sections of "text" (or symbolic nodes) past the finish, so this synthesizes new shows.

Humans process more complex experiences than just visual senses: humans also have layers of numerous [auditory cortex](https://wikipedia.org/wiki/Auditory_cortex) tissues, so that sound compresses, plus a [thalamus](https://wikipedia.org/wiki/Thalamus) (which merges your numerous senses, thus the hippocampus has both audio+visual to access and compress, which, for a computer, would be as if you could all speech + lip motions down to the subtitles ([.ass](https://wikipedia.org/wiki/.ass))).

Sources: <https://wikipedia.org/wiki/Visual_cortex>, [*Neuroscience for Dummies*](https://www.amazon.com/Neuroscience-Dummies-Frank-Amthor/dp/1394171218) plus numerous such documents.

______

Found 2 `arxiv.org` documents (\[1\]\[2\]) through searches for finished programs which do this (but is difficult to follow thus documents, do not know if thus discuss actual finished programs):
- \[1\] [A Computationally Efficient Neural Video Compression Accelerator Based on a Sparse CNN-Transformer Hybrid Network](https://arxiv.org/html/2312.10716v1)
- \[2\] [Advances In Video Compression System Using Deep Neural Network: A Review And Case Studies](https://arxiv.org/abs/2101.06341)

If not, for [sponsorship](https://github.com/SwuduSusuwu/SusuLib/blob/preview/README.md#sponsor) (or [escrow](https://github.com/SwuduSusuwu/SusuLib/blob/preview/README.md#escrow)) will produce this.

Because the `arxiv.org` pages do not list compression ratios, have doubts that those researchers have narrowed down specifics of what to do, but if someone has produced this, won't waste resources to reproduce this.

______

## Compression plus synthesis
Expected compression ratios for this: process *1024\*1280@60fps* (*standard definition*, *2.6 gigabits-per-second*) into compressed structures (such as text) which just use a *few kilobits-per-second*, then reproduce the originals from thus compressed structures (with small [perceptual losses](https://deepai.org/machine-learning-glossary-and-terms/perceptual-loss-function).)
- So compression is around "2,600,000 to 2" (as opposed to [`x264` which is at best “700 to 2”](https://stackoverflow.com/questions/5024114/suggested-compression-ratio-with-h-264)).

Those compression ratio guesses are based on the amount of visuals which human cortices store (plus reproduce close to the original); guess is that most of what humans view involves optical bandwidth of around 2gbps, but is compressed into the neocortex (into formats similar to text at around 2kbps, but not tokenized such as text is; neural tissue produces "circuits" which are more symbolic / conceptual), plus can reproduce around 82% of the specifics of sources (but losses are flattened, thus [you cannot know what was forgotten or misremembered](https://github.com/SwuduSusuwu/SusuPosts/blob/preview/posts/ConsumerWarnings/Jurisprudence_tools_which_forge_humans.md#fallible-witnesses)):
- This section was introduced in response to [_Solar-Pro-2_'s review](https://github.com/SwuduSusuwu/SusuLib/issues/2#issuecomment-3141463645) (which suggests to give specifics of the compression ratio guesses).
- Losses in simple computer visual formats (such as `mpeg-2`) cause noticeable "artifacts" ([accidental corruption](https://wikipedia.org/wiki/Compression_artifact)) such that viewers knew that the throughput was not sufficient; knew that specifics were lost/altered --- [accidental misidentification is common since human neural tissue does not have this attribute](https://swudususuwu.substack.com/p/do-some-jurisdictions-still-trust#%C2%A7fallible-witnesses).
- Expect huge compression ratios due to the fact that human neural systems are not mature until given numerous years of training datasets. Guess that artificial intelligences which have processed years of visuals + sounds will have produced databases (of primitives symbols, concepts, meshes, ...) which allow compression ratios of close to 1,000,000 -> 2 for large visual inputs which are not random. But due to how lossy biological neural tissue is, such super-compressed records can not come close to 100% accurate, so traditional formulas will still have use.
- This will process input such as [_Fanuc_'s videos of somewhat-autonomous tools](https://youtu.be/7lI-PY7InV8) into text (or symbolic / conceptual nodes) which allow to insert such as `Plus, produces houses.`, to produce outputs which shows such tools produce houses.
  - Or process input such as [this show about simple autonomous tools which mass-produce simple autonomous tools](https://youtu.be/hLDbRm-98cs), once processed, can accept inserts such as `As opposed to the standalone robotic arms shown, produces tools with locomotive systems + 2 arms`, which will output a show about such improved tools.

******************************************
# Synopsis + similar posts
- [\[Preview\] How to use _FLOSS_ code to produce autonomous _Arduino_/_Elegoo_ tools](./ArduinoElegooTools.md)
- [\[Preview\] How to produce general-use autonomous tools through calculus (continuous formulas), + use TensorFlow for synthesis of close-to-human consciousness.](https://github.com/SwuduSusuwu/SusuPosts/blob/preview/posts/Autonomous-tools_+_human-consciousness.md)
- [How to improve how fast desktops/laptops/phones execute code](./SimdGpgpuTpu.md)
- [`../SusuPosts/posts/Tensorflow_produces_meshes.md`](https://github.com/SwuduSusuwu/SusuPosts/blob/preview/posts/Tensorflow_produces_meshes.md) discusses how to give simple textual descriptions to `tensorflow` to produce meshes (produce edges plus vertices).
- [*Microsoft*'s "*Project Muse*"](https://www.microsoft.com/en-us/research/blog/introducing-muse-our-first-generative-ai-model-designed-for-gameplay-ideation/) uses artificial neural networks to "guess" the rest of some segment of motion picture. Instead of resolution compression, *Muse* is temporal compression (the examples show that *Muse* can compress motion pictures to a ninth of their original length, then decompress thus into the original motion pictures).
  - *Muse*'s "temporal compression" is more close to `CnsCompress.md` "symbolic/conceptual compression" than `x264`'s "resolution compression" is.
  - To use *Microsoft*'s *Muse* for educational [synthesis](#Compression-plus-synthesis), all you must do is produce the first few seconds of the new mode: *Muse* will "guess" how to finish thus.

Previous posts about artificial neural tissue:
- [Destructive (scan uses nanoscopic decomposition) upload of human's consciousness](https://swudususuwu.substack.com/p/destructive-unreversible-upload-of)
- [Want this physical form gone; am jealous of robots](https://swudususuwu.substack.com/p/want-this-physical-form-gone-so-wont)
- [Neural resources](./VirusAnalysis.md#neural-resources)
- [Albatross executes more compute per unit of neural volume (opposed to humans); howto base artificial neural systems on albatross neural layout/tissue.](./AlbatrossCNS.md)

******************************************

Unknown sources (can not discern truth (fitness-to-use) of those) which have to do with neuromorphic computers:
- [_International Centre for Neuromorphic Systems_](https://www.westernsydney.edu.au/icns)
- [Neuromorphic Cognitive Systems | _Institute of Neuroinformatics_](https://www.ini.uzh.ch/en/research/groups/ncs.html)
- [Neuromorphic systems | _NIST_](https://www.nist.gov/programs-projects/neuromorphic-systems)

