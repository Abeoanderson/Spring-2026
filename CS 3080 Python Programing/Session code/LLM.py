# Install required libraries first (only once)
# pip install torch transformers
from transformers import AutoTokenizer, AutoModelForCausalLM
import torch
from huggingface_hub import list_models
models = list_models(filter="text-generation")
# Filter for models with "gpt" in their name or architecture
gpt_models = []
input("attempting to show all available gpt models for text generation, press any key to continue:")
for model in models:
    if "gpt" in model.modelId.lower():
        gpt_models.append(model.modelId)

# Print all GPT models compatible with AutoModelForCausalLM
for m in gpt_models:
    print(m)
# Load tokenizer and model
print("\n All models showed above are available for text generation using AutoModelForCausalLM\n")

model_name = "distilgpt2" # Small, fast GPT2 variant

print(f"Loading model: {model_name}")

tokenizer = AutoTokenizer.from_pretrained(model_name)
model = AutoModelForCausalLM.from_pretrained(model_name)
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model = model.to(device)
print("model loaded and ready for text generation. \n")
# Prepare input prompt
prompt = input("Enter a text to be completed or ask a question. Type exit to exit the program:\n")
def text_gen(prompt):
    inputs = tokenizer(prompt, return_tensors="pt").to(device)

    # Generate continuation
    outputs = model.generate(
        **inputs,
        max_length=50, # total length including prompt
        do_sample=True, # sampling instead of greedy decoding
        top_k=50, # limits to top-k tokens
        top_p=0.95, # nucleus sampling
        temperature=0.7, # controls randomness
        num_return_sequences=1 # number of outputs
    )

    # Decode and print result
    generated_text = tokenizer.decode(outputs[0],
    skip_special_tokens=True)
    return generated_text

while prompt != "exit":
    generated_text = text_gen(prompt)
    print(repr(generated_text))
    prompt = input("Enter a text to be completed or ask a question. Type exit to exit the program:\n")