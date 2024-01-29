#ifndef CONTENT_MANAGER_H
#define CONTENT_MANAGER_H

#include "content.h"

class ContentManager
{
public:
	// Getters for previous dialog
	Content* get_previous();

	// Getters for current dialog
	Content* get_current();

	// Getters for last dialog
	Content* get_last();

	// Save modifications to the current dialog interface
	void save();

	// Check if data is empty
	[[nodiscard]] bool empty() const;

	// Clear data
	void clear();

	// Update data
	void set_data(const DialogueDataType&);

	// Get data
	[[nodiscard]] DialogueDataType get_data() const;

	// Get current id
	[[nodiscard]] std::string get_id() const;

	// Update current id
	void set_id(const std::string&);

	// Get current section name
	[[nodiscard]] std::string get_section() const;

	// Set current section name
	void set_section(const std::string& = "");

	// Remove section
	void remove_section(const std::string& = "");

	// Get section content
	[[nodiscard]] SectionDataType& get_section_contents(const std::string& = "");

	// Set section content
	void set_section_contents(const SectionDataType&, const std::string& = "");

	// Get dialog data
	[[nodiscard]] ContentDataType& get_content(const std::string& = "", const std::string& = "");

	// Remove dialog data
	void remove_content(const std::string& = "", const std::string& = "");

private:
	DialogueDataType dialog_data_;
	std::string section_;
	std::string id_ = "head";
	Content* current_ = nullptr;
	Content* last_ = nullptr;
	Content* previous_ = nullptr;
	// If the pointer to the current dialog data does not exist, set_data the pointer
	void refresh_current();
};

#endif
